#include "SceneOil.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;


#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

namespace imat3111
{

	SceneOil::SceneOil() : width(800), height(600)
	{}

	void SceneOil::initScene(QuatCamera camera)
	{
		compileAndLinkShader();

		gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		gl::Enable(gl::DEPTH_TEST);

		//Set up the lighting
		setLightParams(camera);

		//Create the plane to represent the ground
		plane = new VBOPlane(20.0, 20.0, 20, 20);

		Bitmap texture = Bitmap::bitmapFromFile("Forest.jpg");
		//texture.flipVertically();
		gTexture = new Texture(texture);

		setupFBO();

		// Array for full-screen quad
		GLfloat verts[] = {
			-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
		};
		GLfloat tc[] = {
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		// Set up the buffers

		unsigned int handle[2];
		gl::GenBuffers(2, handle);

		gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
		gl::BufferData(gl::ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, gl::STATIC_DRAW);

		gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
		gl::BufferData(gl::ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, gl::STATIC_DRAW);

		// Set up the vertex array object

		gl::GenVertexArrays(1, &fsQuad);
		gl::BindVertexArray(fsQuad);

		gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
		gl::VertexAttribPointer((GLuint)0, 3, gl::FLOAT, FALSE, 0, ((GLubyte *)NULL + (0)));
		gl::EnableVertexAttribArray(0);  // Vertex position

		gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
		gl::VertexAttribPointer((GLuint)2, 2, gl::FLOAT, FALSE, 0, ((GLubyte *)NULL + (0)));
		gl::EnableVertexAttribArray(2);  // Texture coordinates

		gl::BindVertexArray(0);

		// Set up the subroutine indexes
		GLuint programHandle = prog.getHandle();
		pass1Index = gl::GetSubroutineIndex(programHandle, gl::FRAGMENT_SHADER, "pass1");
		pass2Index = gl::GetSubroutineIndex(programHandle, gl::FRAGMENT_SHADER, "pass2");

		prog.setUniform("Width", 800);
		prog.setUniform("Height", 600);
		prog.setUniform("EdgeThreshold", 0.1f);
		prog.setUniform("RenderTex", 0);
		prog.setUniform("radius", 5);
		prog.setUniform("intensityLevel", 20);
	}

	void SceneOil::setupFBO() {
		// Generate and bind the framebuffer
		gl::GenFramebuffers(1, &fboHandle);
		gl::BindFramebuffer(gl::FRAMEBUFFER, fboHandle);

		// Create the texture object
		GLuint renderTex;
		gl::GenTextures(1, &renderTex);
		gl::ActiveTexture(gl::TEXTURE0);  // Use texture unit 0
		gl::BindTexture(gl::TEXTURE_2D, renderTex);
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST);

		// Bind the texture to the FBO
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, renderTex, 0);

		// Create the depth buffer
		GLuint depthBuf;
		gl::GenRenderbuffers(1, &depthBuf);
		gl::BindRenderbuffer(gl::RENDERBUFFER, depthBuf);
		gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, width, height);

		// Bind the depth buffer to the FBO
		gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT,
			gl::RENDERBUFFER, depthBuf);

		// Set the targets for the fragment output variables
		GLenum drawBuffers[] = { gl::COLOR_ATTACHMENT0 };
		gl::DrawBuffers(1, drawBuffers);

		// Unbind the framebuffer, and revert to default framebuffer
		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	}

	void SceneOil::update(float t)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneOil::setLightParams(QuatCamera camera)
	{

		vec3 worldLight = vec3(10.0f, 10.0f, 10.0f);

		prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
		prog.setUniform("Light.Position", vec4(worldLight, 1.0));

	}

	void SceneOil::render(QuatCamera camera)
	{
		pass1(camera);
		pass2(camera);
	}

	void SceneOil::pass1(QuatCamera camera)
	{
		gl::BindFramebuffer(gl::FRAMEBUFFER, fboHandle);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &pass1Index);
		gl::ActiveTexture(gl::TEXTURE1);
		gl::BindTexture(gl::TEXTURE_2D, gTexture->object());
		prog.setUniform("diffuseTexture", 1);
		//Initialise the model matrix for the plane
		model = mat4(1.0f);
		model = glm::rotate(glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		setMatrices(camera);
		//Set the plane's material properties in the shader and render
		prog.setUniform("Material.Ka", 0.7f / 7.0f, 1.0f / 7.0f, 0.7f / 7.0f);
		prog.setUniform("Material.Kd", 0.7f, 1.0f, 0.7f);
		prog.setUniform("Material.Ks", 0.7f, 1.0f, 0.7f);
		prog.setUniform("Material.Shininess", 1.0f);
		plane->render();

	}

	void SceneOil::pass2(QuatCamera camera)
	{
		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		gl::UniformSubroutinesuiv(gl::FRAGMENT_SHADER, 1, &pass2Index);

		prog.setUniform("postProcess", m_animate);

		model = mat4(1.0);
		mat4 view = mat4(1.0);
		mat4 projection = mat4(1.0);
		//glm::lookAt(vec3(0.0,0.0,5.0), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f));
		prog.setUniform("ModelViewMatrix", view*model);
		prog.setUniform("NormalMatrix",
			mat3(vec3(view[0]), vec3(view[1]), vec3(view[2])));
		prog.setUniform("ProjectionMatrix", projection);
		prog.setUniform("MVP", projection * view);

		// Render the full-screen quad
		gl::BindVertexArray(fsQuad);
		gl::DrawArrays(gl::TRIANGLES, 0, 6);
	}

	void SceneOil::setMatrices(QuatCamera camera)
	{


		mat4 mv = camera.view() * model;
		prog.setUniform("ModelViewMatrix", mv);
		prog.setUniform("NormalMatrix",
			mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("ProjectionMatrix", camera.projection());
		prog.setUniform("MVP", camera.projection() * mv);

	}

	void SceneOil::resize(QuatCamera camera, int w, int h)
	{
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;

		camera.setAspectRatio((float)w / h);

	}

	void SceneOil::compileAndLinkShader()
	{
		try {
			prog.compileShader("./Shaders/oil.vs");
			prog.compileShader("./Shaders/oil.fs");
			//prog.compileShader("./Shaders/kuwahara.fs");
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