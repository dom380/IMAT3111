#pragma once
#ifndef SCENENORMALMAP_H
#define SCENENORMALMAP_H

#include "gl_core_4_3.hpp"
#include "scene.h"
#include "QuatCamera.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "vboplane.h"
#include "vbomesh.h"
#include "Texture.h"
#include "Bitmap.h"
using imat3111::QuatCamera;

class SceneNormalMap : public imat3111::Scene
{
public:
	/**
	Load textures, initialize shaders, etc.
	*/
	void initScene(QuatCamera camera);

	/**
	This is called prior to every frame.  Use this
	to update your animation.
	*/
	// virtual void update(GLFWwindow * window, float t ) = 0;

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

	VBOPlane *plane;  //Plane VBO

	VBOMesh *mesh;

	Texture *gTextureDiffuse;
	Texture *gTextureNormal;

	glm::mat4 model; //Model matrix

	bool useNormals = true;


	void setMatrices(QuatCamera camera); //Set the camera matrices

	void compileAndLinkShader(); //Compile and link the shader
};

#endif // !SCENENORMALMAP_H