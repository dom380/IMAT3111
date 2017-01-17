// Shader Lab.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "gl_core_4_3.hpp"

#include <glfw3.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//Select the 4.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Start the OpenGL context and open a window using the
	//GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		glfwTerminate();
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(640, 480, "First GLSL Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	//Load the OpenGL functions for C++
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	if (!didLoad) {
		//Load failed
		fprintf(stderr, "ERROR: GLLoadGen failed to load functions\n");
		glfwTerminate();
		return 1;
	}
	printf("Number of functions that failed to load : %i.\n",
		didLoad.GetNumMissing());
	//Tell OpenGL to only draw a pixel if its shape is closer to
	//the viewer
	//i.e. Enable depth testing with smaller depth value
	//interpreted as being closer
	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);
	//Set up the vertices for a triangle
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	//Create a vertex buffer object to hold this data
	GLuint vbo = 0;
	gl::GenBuffers(1, &vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, 9 * sizeof(float), points,
		gl::STATIC_DRAW);
	//Create a vertex array object
	GLuint vao = 0;
	gl::GenVertexArrays(1, &vao);
	gl::BindVertexArray(vao);
	gl::EnableVertexAttribArray(0);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, NULL);
	//The shader code strings which later we will put in
	//separate files
	//The Vertex Shader
	const char* vertex_shader = "#version 400\n"
		"in vec3 up;"
		"vec3 finalPos;"
		"void main() {"
		"	finalPos = up;"
		"	finalPos.y = up.y - 0.5;"
		"	gl_Position = vec4(up,1.0);"
		"}";
	//The Fragment Shader
	const char* fragment_shader = "#version 400\n"
		"out vec4 frag_colour;\n"
		"void main() {\n"
		"	frag_colour = vec4(1.0,0.5,0.0,1.0);"
		"}";
	//Load the strings into shader objects and compile
	GLuint vs = gl::CreateShader(gl::VERTEX_SHADER);
	gl::ShaderSource(vs, 1, &vertex_shader, NULL);
	gl::CompileShader(vs);
	GLuint fs = gl::CreateShader(gl::FRAGMENT_SHADER);
	gl::ShaderSource(fs, 1, &fragment_shader, NULL);
	gl::CompileShader(fs);
	//Compiled shaders must be compiled into a single executable
	//GPU shader program
	//Create empty program and attach shaders
	GLuint shader_program = gl::CreateProgram();
	gl::AttachShader(shader_program, fs);
	gl::AttachShader(shader_program, vs);
	gl::LinkProgram(shader_program);
	//Now draw
	while (!glfwWindowShouldClose(window)) {
		//Clear the drawing surface
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		gl::UseProgram(shader_program);
		gl::BindVertexArray(vao);
		//Draw point 0 to 3 from the currently bound VAO with
		//current in-use shader
		gl::DrawArrays(gl::TRIANGLES, 0, 3);
		//update GLFW event handling
		glfwPollEvents();
		//Put the stuff we have been drawing onto the display
		glfwSwapBuffers(window);
	}
	//Close GLFW and end
	glfwTerminate();
	return 0;
}