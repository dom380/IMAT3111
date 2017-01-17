#pragma once
#ifndef SALTIRESCENE_H
#define SALTIRESCENE_H

#include "scene.h"
#include "gl_core_4_3.hpp"
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <string>
using std::string;
#include <vector>
using std::vector;


class SaltireScene : public Scene
{
public:
	SaltireScene(int windowWidth, int windowHeight);

	// Inherited via Scene
	/**
	Load textures, initialize shaders, etc.
	*/
	void initScene();

	/**
	This is called prior to every frame.  Use this
	to update your animation.
	*/
	void update(float t);

	/**
	Draw your scene.
	*/
	void render();

	/**
	Called when screen is resized
	*/
	void resize(int w, int h);

	void linkMe(GLint vertShader, GLint fragShader);
private:
	size_t indexSize;
	GLuint vaoHandle;
	GLuint programHandle;
	int width;
	int height;
	glm::mat4 modelView = glm::mat4();
	glm::mat4 view = glm::mat4();
	glm::mat4 perspective = glm::mat4();
	float angle = 0;
	GLint modelMatrixID;
	GLint viewMatrixID;
	GLint projectionMatrixID;
};

#endif