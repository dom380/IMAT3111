#ifndef SCENESHADOWMAP_H
#define SCENESHADOWMAP_H


#include "gl_core_4_3.hpp"
#include <glfw3.h>

#include "scene.h"
#include "glslprogram.h"


#include "vbocube.h"


#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

namespace imat3111
{

	class SceneShadowMap : public Scene
	{
	private:
		GLSLProgram prog, solidProg;
		GLuint shadowFBO, pass1Index, pass2Index;


		VBOCube *cube;

		int width, height;
		int shadowMapWidth, shadowMapHeight;

		vec3 lightPos;
		mat4 model, view, projection;
		mat4 lightPV;
		mat4 shadowBias;


		void setMatrices(mat4 model, mat4 view, mat4 projection);
		void compileAndLinkShader();
		void setupFBO();
		void drawScene(mat4 view, mat4 projection);




	public:
		SceneShadowMap();

		void initScene(QuatCamera camera);
		void render(QuatCamera camera);
		void resize(QuatCamera camera, int, int);

	};
}

#endif // SCENESHADOWMAP_H