#include "..\include\ParallaxScene.h"

ParallaxScene::ParallaxScene()
{
}

void ParallaxScene::initScene(QuatCamera camera)
{

	//|Compile and link the shader  
	compileAndLinkShader();

	gl::Enable(gl::DEPTH_TEST);
	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::BACK);

	//Set up the lighting
	setLightParams(camera);

	//read model data
	mesh = new VBOMesh("./cube.obj", false, true, true);
	Bitmap texture = Bitmap::bitmapFromFile("cobblesDiffuse.png");
	texture.flipVertically();
	gTextureDiffuse = new Texture(texture);
	Bitmap normalMap = Bitmap::bitmapFromFile("cobblesNormal.png");
	normalMap.flipVertically();
	gTextureNormal = new Texture(normalMap);
	Bitmap specularMap = Bitmap::bitmapFromFile("cobblesSpecular.png");
	specularMap.flipVertically();
	gTextureSpecular = new Texture(specularMap);
	Bitmap heightMap = Bitmap::bitmapFromFile("cobblesHeight.png");
	heightMap.flipVertically();
	gTextureHeight = new Texture(heightMap);
}

void ParallaxScene::render(QuatCamera camera)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	model = mat4(1.0f);
	setMatrices(camera);
	prog.setUniform("Kd", 1.0f, 1.0f, 1.0f);
	prog.setUniform("Ks", 0.8f, 0.8f, 0.8f);
	prog.setUniform("matSpec", 40.0f);
	prog.setUniform("heightScale", 0.1f);

	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, gTextureDiffuse->object());
	prog.setUniform("diffuseTexture", 0);
	gl::ActiveTexture(gl::TEXTURE1);
	gl::BindTexture(gl::TEXTURE_2D, gTextureNormal->object());
	prog.setUniform("normalTexture", 1);
	gl::ActiveTexture(gl::TEXTURE2);
	gl::BindTexture(gl::TEXTURE_2D, gTextureSpecular->object());
	prog.setUniform("specularTexture", 2);
	gl::ActiveTexture(gl::TEXTURE3);
	gl::BindTexture(gl::TEXTURE_2D, gTextureHeight->object());
	prog.setUniform("depthTexture", 3);

	
	mesh->render();
}

void ParallaxScene::resize(QuatCamera camera, int, int)
{
}

void ParallaxScene::setLightParams(QuatCamera camera)
{
	vec3 worldLight = vec3(0.0f, 5.0f, 20.0f);
	prog.setUniform("Ld", 1.0f, 1.0f, 1.0f); //Diffuse intensity
	prog.setUniform("LightPosition", worldLight);
	prog.setUniform("Ia", glm::vec3(0.05, 0.05, 0.05)); //Ambient intensity
	prog.setUniform("a", 1000.f); //Attenuation distance
	prog.setUniform("f", 1.0f); //Attenuation falloff
}

void ParallaxScene::setMatrices(QuatCamera camera)
{
	mat4 mv = camera.view() * model;
	prog.setUniform("ModelViewMatrix", mv);
	//prog.setUniform("NormalMatrix",
	//	mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", camera.projection() * mv);
	// the correct matrix to transform the normal is the transpose of the inverse of the M matrix
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	prog.setUniform("M", model);
	prog.setUniform("NormalMatrix", normMat);
	prog.setUniform("V", camera.view());
	prog.setUniform("P", camera.projection());
	prog.setUniform("viewPos", camera.position());
}

void ParallaxScene::compileAndLinkShader()
{
	try {
		prog.compileShader("shaders/parallax.vert");
		prog.compileShader("shaders/parallax.frag");
		prog.link();
		prog.validate();
		prog.use();
	}
	catch (GLSLProgramException & e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}
