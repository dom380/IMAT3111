#include "..\include\SceneNormalMap.h"
#include <iostream>
#include "GLSupport.h"

void SceneNormalMap::initScene(QuatCamera camera)
{
	//|Compile and link the shader  
	compileAndLinkShader();

	gl::Enable(gl::DEPTH_TEST);

	//Set up the lighting
	setLightParams(camera);


	//Create the plane to represent the ground
	//plane = new VBOPlane(100.0, 100.0, 100, 100);
	//Bitmap normalMap = Bitmap::bitmapFromFile("brickwall_normal.jpg");
	//Bitmap texture = Bitmap::bitmapFromFile("brickwall.jpg");

	mesh = new VBOMesh("bs_ears.obj", false, true, true);

	Bitmap texture = Bitmap::bitmapFromFile("ogre_diffuse.png");
	texture.flipVertically();
	gTextureDiffuse = new Texture(texture);
	Bitmap normalMap = Bitmap::bitmapFromFile("ogre_normalmap.png");
	normalMap.flipVertically();
	gTextureNormal = new Texture(normalMap);
}

void SceneNormalMap::render(QuatCamera camera)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	model = mat4(1.0f);
	setMatrices(camera);
	prog.setUniform("Kd", 0.6f, 0.6f, 0.6f);
	prog.setUniform("Ks", 0.0f, 0.0f, 0.0f);
	prog.setUniform("n", 0.0f);
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, gTextureDiffuse->object());
	prog.setUniform("diffuseTexture", 0);
	gl::ActiveTexture(gl::TEXTURE1);
	gl::BindTexture(gl::TEXTURE_2D, gTextureNormal->object());
	prog.setUniform("normalTexture", 1);
	mesh->render();
	auto error = OpenGLSupport().GetError();
}

void SceneNormalMap::resize(QuatCamera camera, int, int)
{
}

void SceneNormalMap::setLightParams(QuatCamera camera)
{
	vec3 worldLight = vec3(0.0f, 6.0f, 7.0f);

	prog.setUniform("Ld", 1.0f, 1.0f, 1.0f); //Diffuse intensity
											 //  prog.setUniform("LightPosition", camera.view() * vec4(worldLight,1.0) );
	prog.setUniform("LightPosition", worldLight);
	prog.setUniform("Ia", glm::vec3(0.05, 0.05, 0.05)); //Ambient intensity
	prog.setUniform("a", 1000.f); //Attenuation distance
	prog.setUniform("f", 1.0f); //Attenuation falloff
}


void SceneNormalMap::setMatrices(QuatCamera camera)
{
	mat4 mv = camera.view() * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",
		mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", camera.projection() * mv);
	// the correct matrix to transform the normal is the transpose of the inverse of the M matrix
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	prog.setUniform("M", model);
	//prog.setUniform("NormalMatrix", normMat);
	prog.setUniform("V", camera.view());
	prog.setUniform("P", camera.projection());
	prog.setUniform("viewPos", camera.position());
}

void SceneNormalMap::compileAndLinkShader()
{
	try {
		prog.compileShader("Shaders/normal.vert");
		prog.compileShader("Shaders/normal.frag");
		prog.link();
		prog.validate();
		prog.use();
	}
	catch (GLSLProgramException & e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}
