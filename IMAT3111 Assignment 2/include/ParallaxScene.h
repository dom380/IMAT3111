#pragma once
#include "gl_core_4_3.hpp"
#include "scene.h"
#include "QuatCamera.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Texture.h"
#include "Bitmap.h"
#include "ObjReader.h"
#include "vbomesh.h"
#include <vector>
using imat3111::Scene;
using imat3111::QuatCamera;
using std::vector;

class ParallaxScene : public Scene
{
public:
	ParallaxScene();
	/**
	Load textures, initialize shaders, etc.
	*/
	void initScene(QuatCamera camera);

	/**
	Draw your scene.
	*/
	void render(QuatCamera camera);

	/**
	Called when screen is resized
	*/
	void resize(QuatCamera camera, int, int);

	void setLightParams(QuatCamera camera); //Setup the lighting

private:
	GLSLProgram prog;

	int width, height;

	Texture *gTextureDiffuse;
	Texture *gTextureNormal;
	Texture *gTextureSpecular;
	Texture *gTextureHeight;
	VBOMesh *mesh;

	glm::mat4 model; //Model matrix

	bool useNormals = true;


	void setMatrices(QuatCamera camera); //Set the camera matrices

	void compileAndLinkShader(); //Compile and link the shader
};
