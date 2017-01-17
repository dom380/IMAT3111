#ifndef SCENEBASIC_H
#define SCENEBASIC_H



#include "scene.h"

#include "gl_core_4_3.hpp"

class SceneBasic : public Scene
{
private:
	int width, height;
	GLuint vboHandles[2];
	GLuint vbo;
	GLuint vaoHandle;
	GLuint programHandle;

	void linkMe(GLint vertShader, GLint fragShader);

public:
	SceneBasic();

	void initScene();
	void update( float t);
	void render();
	void resize(int, int);

};
#endif  //SCENEBASIC_H