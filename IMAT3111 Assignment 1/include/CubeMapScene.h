#pragma once
#ifndef CUBEMAPSCENE_H
#define CUBEMAPSCENE_H
#include "glslprogram.h"
#include "scene.h"
using imat3111::Scene;
#include "QuatCamera.h"
using imat3111::QuatCamera;
#include <vector>
using std::vector;
#include "vboteapot.h"
#include <iostream>
#include "Bitmap.h"

class CubeMapScene : public Scene {
public:
	void initScene(QuatCamera camera);
	void render(QuatCamera camera);
	void resize(QuatCamera camera, int w, int h);

private:
	GLSLProgram teapotShader;
	GLSLProgram skyboxShader;
	int width, height;

	VBOTeapot *teapot;  //Teapot VBO

	mat4 model; //Model matrix
	vector<GLfloat> skyboxVertices;
	GLuint textureID;

	GLuint vaoHandle;
	GLuint indexSize;

	float angle =0 ;

	void setMatrices(QuatCamera camera, GLSLProgram* shader); //Set the camera matrices

	void compileAndLinkShader(); //Compile and link the shaders

	void buildSkybox();

};

#endif