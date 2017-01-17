#pragma once
#ifndef SCENESPOTLIGHT_H
#define SCENESPOTLIGHT_H
#include "gl_core_4_3.hpp"
#include "scene.h"
#include "QuatCamera.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "vboteapot.h"
#include "vboplane.h"

using imat3111::Scene;
using imat3111::QuatCamera;

class SceneSpotLight : public Scene
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

	VBOTeapot *teapot;  //Teapot VBO
	VBOPlane *plane;  //Plane VBO

	glm::mat4 model; //Model matrix


	void setMatrices(QuatCamera camera); //Set the camera matrices

	void compileAndLinkShader(); //Compile and link the shader
};

#endif // !SCENESPOTLIGHT_H
