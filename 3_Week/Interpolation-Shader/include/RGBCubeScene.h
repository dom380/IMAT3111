#pragma once
#ifndef RGBCUBESCENE_H
#define RGBCUBESCENE_H

#include "stdafx.h"

#include "scene.h"
#include "gl_core_4_3.hpp"


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

class RGBCubeScene : public Scene
{
public:
	RGBCubeScene();

	RGBCubeScene(bool outlineEnabled);

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
	int width, height;
	GLuint vboHandles[2];
	GLuint vbo;
	GLuint vaoHandle;
	GLuint programHandle;
	size_t indexSize;
	bool outlineEnabled = false;

};



#endif