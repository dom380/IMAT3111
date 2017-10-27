#ifndef SCENEDEFERRED_H
#define SCENEDEFERRED_H

#include "gl_core_4_3.hpp"
#include "scene.h"
#include "glslprogram.h"

#include "vboplane.h"

#include "vboteapot.h"



#include <glm/glm.hpp>
using glm::mat4;

namespace imat3111
{

	class SceneDeferred : public Scene
	{
	private:
		GLSLProgram prog;

		int width, height;
		GLuint deferredFBO;
		GLuint quad, pass1Index, pass2Index;

		VBOPlane *plane;

		VBOTeapot *teapot;

		mat4 model;

		void setMatrices(QuatCamera camera);
		void compileAndLinkShader();
		void setupFBO();
		void pass1(QuatCamera camera);
		void pass2(QuatCamera camera);

	public:
		SceneDeferred();


		void setLightParams(QuatCamera camera); //Setup the lighting

		void initScene(QuatCamera camera); //Initialise the scene

		void update(float t); //Update the scene

		void render(QuatCamera camera);	//Render the scene

		void resize(QuatCamera camera, int, int); //Resize

	};
}

#endif // SCENEDEFERRED_H