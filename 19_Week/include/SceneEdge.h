#pragma once
#ifndef SCENEEDGE_H
#define SCENEEDGE_H

#include "gl_core_4_3.hpp"

#include "scene.h"
#include "glslprogram.h"

#include "vboplane.h"


#include "vboteapot.h"

#include <glm/glm.hpp>
using glm::mat4;

namespace imat3111
{

	class SceneEdge : public Scene
	{
	private:
		GLSLProgram prog;

		int width, height;
		GLuint fsQuad, pass1Index, pass2Index, fboHandle;

		VBOPlane *plane;

		VBOTeapot *teapot;


		mat4 model;



		void setMatrices(QuatCamera camera);
		void compileAndLinkShader();
		void setupFBO();
		void pass1(QuatCamera camera);
		void pass2(QuatCamera camera);

	public:
		SceneEdge();

		void setLightParams(QuatCamera camera); //Setup the lighting

		void initScene(QuatCamera camera);
		void update(float t);
		void render(QuatCamera camera);
		void resize(QuatCamera camera, int, int);
	};

}
#endif // SCENEEDGE_H