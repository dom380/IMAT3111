#include "stdafx.h"
#include "..\include\SceneTexture.h"

#include "scenebasic.h"

#include <cstdio>
#include <cstdlib>


#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <string>
using std::string;


//#define SINGLE_BUFFER

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void SceneTexture::linkMe(GLint vertShader, GLint fragShader)
{
	// Create the program object
	programHandle = gl::CreateProgram();
	if (0 == programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}


	// Attach the shaders to the program object
	gl::AttachShader(programHandle, vertShader);
	gl::AttachShader(programHandle, fragShader);

	// Link the program
	gl::LinkProgram(programHandle);

	// Check for successful linking
	GLint status;
	gl::GetProgramiv(programHandle, gl::LINK_STATUS, &status);
	if (FALSE == status) {

		fprintf(stderr, "Failed to link shader program!\n");

		GLint logLen;
		gl::GetProgramiv(programHandle, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetProgramInfoLog(programHandle, logLen, &written, log);

			fprintf(stderr, "Program log: \n%s", log);

			free(log);
		}
	}
	else {
		gl::UseProgram(programHandle);
	}

}

SceneTexture::SceneTexture()
{
}

void SceneTexture::initScene()
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	ifstream inFile("shader/basic.vert");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader/basic.vert\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());

	// Create the shader object
	GLuint vertShader = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = { codeStr.c_str() };
	gl::ShaderSource(vertShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(vertShader);

	// Check compilation status
	GLint result;
	gl::GetShaderiv(vertShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Vertex shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(vertShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(vertShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shaderCode here
	ifstream fragFile("shader/basic.frag");
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader/basic.frag\n");
		exit(1);
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (0 == fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	gl::ShaderSource(fragShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(fragShader);

	// Check compilation status
	gl::GetShaderiv(fragShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Fragment shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(fragShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	linkMe(vertShader, fragShader);

	/////////////////// Create the VBO ////////////////////
	vector<GLfloat> positionData = {
		-0.8f, -0.6f, 0.0f,
		0.8f, -0.6f, 0.0f,
		0.8f,  0.6f, 0.0f,
		-0.8f, 0.6f, 0.0f
	};
	vector<GLfloat> uvData = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 1.0f
	};
	vector<GLubyte> indices = {
		0,1,2,
		0,3,2
	};

	GLuint vboHandles[3];
	gl::GenBuffers(3, vboHandles);

	//Create IBO for triangle
	GLuint indiceBuffer = vboHandles[0];
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * indices.size(), &indices[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);

	//Create VBO for triangle vertices
	GLuint vertexBuffer = vboHandles[1];
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * positionData.size(), &positionData[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VBO for triangle texture coords
	GLuint textCoordBuffer = vboHandles[2];
	gl::BindBuffer(gl::ARRAY_BUFFER, textCoordBuffer);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * uvData.size(), &uvData[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VAO for triangle
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);
	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);

	//Bind VBOs to VAO
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
	gl::BindBuffer(gl::ARRAY_BUFFER, textCoordBuffer);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

	//Bind IBO to VAO
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);

	// Load the texture
	Bitmap bmp = Bitmap::bitmapFromFile("flag.png");
	bmp.flipVertically();
	gTexture = new Texture(bmp);
	//Set texture
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, gTexture->object());
	GLint loc = gl::GetUniformLocation(programHandle, "tex");
	gl::Uniform1f(loc, 0);//WHy are we setting the tex variable to 0????

	gl::BindVertexArray(0);

	indexSize = static_cast<GLsizei>(indices.size());
	
}

void SceneTexture::update(float t)
{
}

void SceneTexture::render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT);
	gl::BindVertexArray(vaoHandle);
	gl::DrawElements(gl::TRIANGLES, indexSize, gl::UNSIGNED_BYTE, BUFFER_OFFSET(0));
	gl::BindVertexArray(0);
}

void SceneTexture::resize(int, int)
{
}
