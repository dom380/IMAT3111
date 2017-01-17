#pragma once
#ifndef SCENETEXTURE_H
#define SCENETEXTURE_H



#include "scene.h"

#include "gl_core_4_3.hpp"
#include "Texture.h"
#include <vector>
using std::vector;

class SceneTexture : public Scene
{
private:
	int width, height;
	GLuint vboHandles[2];
	GLuint vbo;
	GLuint vaoHandle;
	GLuint programHandle;
	Texture* gTexture;
	GLsizei indexSize;

	void linkMe(GLint vertShader, GLint fragShader);

public:
	SceneTexture();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);

};
#endif  //SCENEBASIC_H