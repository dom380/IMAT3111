#include "sceneshadowmap.h"



#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;
using std::cout;

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

namespace imat3111

{
	SceneShadowMap::SceneShadowMap()
	{
		width = 800;
		height = 600;
		shadowMapWidth = 1024;
		shadowMapHeight = 1024;


	}

	void SceneShadowMap::initScene(QuatCamera camera)
	{
		compileAndLinkShader();

		gl::ClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		gl::Enable(gl::DEPTH_TEST);

		//Create a cube to be used for all objects in the scene
		cube = new VBOCube();

		// Set up the FBO
		setupFBO();


		GLuint programHandle = prog.getHandle();
		pass1Index = gl::GetSubroutineIndex(programHandle, gl::FRAGMENT_SHADER, "recordDepth");
		pass2Index = gl::GetSubroutineIndex(programHandle, gl::FRAGMENT_SHADER, "shadeWithShadow");

		//Shadow Bias Matrix
		shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, 0.5f, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.5f, 0.0f),
			vec4(0.5f, 0.5f, 0.5f, 1.0f)
		);

		//Light position in World coords
		lightPos = vec3(10.0f, 10.0f, 10.0f);// World coords

		prog.setUniform("LightIntensity", vec3(1.0f, 1.0f, 1.0f));
		prog.setUniform("LightPosition", vec4(lightPos, 1.0f));

		prog.setUniform("ShadowMap", 0);
	}




	void SceneShadowMap::setupFBO()
	{
		GLfloat border[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		// The depth buffer texture
		GLuint depthTex;
		gl::GenTextures(1, &depthTex);
		gl::BindTexture(gl::TEXTURE_2D, depthTex);
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::DEPTH_COMPONENT, shadowMapWidth,
			shadowMapHeight, 0, gl::DEPTH_COMPONENT, gl::FLOAT, NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
		gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, border);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_COMPARE_MODE, gl::COMPARE_REF_TO_TEXTURE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_COMPARE_FUNC, gl::LESS);

		// Assign the depth buffer texture to texture channel 0
		gl::ActiveTexture(gl::TEXTURE0);
		gl::BindTexture(gl::TEXTURE_2D, depthTex);

		// Create and set up the FBO
		gl::GenFramebuffers(1, &shadowFBO);
		gl::BindFramebuffer(gl::FRAMEBUFFER, shadowFBO);
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT,
			gl::TEXTURE_2D, depthTex, 0);

		GLenum drawBuffers[] = { gl::NONE };
		gl::DrawBuffers(1, drawBuffers);

		GLenum result = gl::CheckFramebufferStatus(gl::FRAMEBUFFER);
		if (result == gl::FRAMEBUFFER_COMPLETE) {
			printf("Framebuffer is complete.\n");
		}
		else {
			printf("Framebuffer is not complete.\n");
		}

		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	}



	void SceneShadowMap::render(QuatCamera camera)
	{
		static int i = 0;
		prog.use();

		// Pass 1 (shadow map generation)
		gl::Enable(gl::CULL_FACE);
		glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.f, 1.0f, 50.f);
		//glm::mat4 projection = glm::perspective(80.0, 1.0, 1.0, 50.0); 
		glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightPV = shadowBias * projection * view;
		gl::CullFace(gl::FRONT);
		gl::BindFramebuffer(gl::FRAMEBUFFER, shadowFBO);
		gl::Clear(gl::DEPTH_BUFFER_BIT);
		gl::Viewport(0, 0, shadowMapWidth, shadowMapHeight);
		gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &pass1Index);
		drawScene(view, projection);
		gl::Flush();
		gl::Finish();
		gl::CullFace(gl::BACK);
		gl::Disable(gl::CULL_FACE);

		// Pass 2 (render the scene with shadows)

		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		gl::Viewport(0, 0, width, height);
		gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &pass2Index);
		drawScene(camera.view(), camera.projection());
	}


	void SceneShadowMap::drawScene(mat4 view, mat4 projection)
	{

		//Green floor made from cube
		vec3 colour = vec3(0.0f, 1.0f, 0.0f);
		prog.setUniform("Material.Ka", colour*0.2f);
		prog.setUniform("Material.Kd", colour);
		prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
		prog.setUniform("Material.Shininess", 1.0f);
		model = mat4(1.0f);
		model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
		model *= glm::scale(vec3(20.0f, 0.1f, 20.0f));
		setMatrices(model, view, projection);
		cube->render();

		//Orange cube object
		colour = vec3(1.0f, 0.5f, 0.0f);
		prog.setUniform("Material.Ka", colour * 0.2f);
		prog.setUniform("Material.Kd", colour);
		prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
		prog.setUniform("Material.Shininess", 150.0f);
		model = mat4(1.0f);
		model *= glm::translate(vec3(0.0f, 1.0f, 0.0f));
		model *= glm::scale(vec3(2.0f, 2.0f, 2.0f));
		setMatrices(model, view, projection);
		cube->render();

		//Magenta cube object
		colour = vec3(1.0f, 0.1f, 1.0f);
		prog.setUniform("Material.Ka", colour * 0.2f);
		prog.setUniform("Material.Kd", colour);
		prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
		prog.setUniform("Material.Shininess", 150.0f);
		model = mat4(1.0f);
		model *= glm::translate(vec3(3.0f, 2.0f, 1.0f));
		model *= glm::scale(vec3(1.0f, 4.0f, 1.0f));
		setMatrices(model, view, projection);
		cube->render();

	}



	void SceneShadowMap::setMatrices(mat4 model, mat4 view, mat4 projection)
	{
		mat4 mv = view * model;
		prog.setUniform("ViewMatrix", view);
		prog.setUniform("ModelViewMatrix", mv);
		prog.setUniform("NormalMatrix",
			mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("MVP", projection * mv);
		prog.setUniform("ShadowMatrix", lightPV * model);

	}


	void SceneShadowMap::resize(QuatCamera camera, int w, int h)
	{
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;
		camera.setAspectRatio((float)w / h);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Compile and link the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneShadowMap::compileAndLinkShader()
	{

		try {
			prog.compileShader("./Shaders/shadowmap.vert");
			prog.compileShader("./Shaders/shadowmap.frag");
			prog.link();
			prog.validate();
			prog.use();
		}
		catch (GLSLProgramException & e) {
			cerr << e.what() << endl;
			exit(EXIT_FAILURE);
		}
	}



}