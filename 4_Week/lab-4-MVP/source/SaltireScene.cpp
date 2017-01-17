#include "stdafx.h"
#include "SaltireScene.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

SaltireScene::SaltireScene(int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

void SaltireScene::initScene()
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	//ifstream inFile("shader/basic.vert");
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
	ifstream fragFile;
	fragFile = ifstream("shader/basic.frag");
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

	vector<float> vertices = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.45f, -0.5f, 0.0f,
		0.0f, -0.05f, 0.0f,
		0.45f, -0.5f, 0.0f,
		-0.45f, 0.5f, 0.0f,
		0.0f, 0.05f, 0.0f,
		0.45f, 0.5f, 0.0f,
		-0.5f, -0.45f, 0.0f,
		-0.05f, 0.0f, 0.0f,
		-0.5f, 0.45f, 0.0f,
		0.5f, -0.45f, 0.0f,
		0.05f, 0.0f, 0.0f,
		0.5f, 0.45f, 0.0f,
	};

	vector<float> colourData = {
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
		0.0f,0.0f,1.f,
	};

	vector<GLubyte> indices = {
		0,1,2,
		0,3,2,
		4,5,6,
		7,8,9,
		10,11,12,
		13,14,15
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
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VBO for triangle colours
	GLuint colourBuffer = vboHandles[2];
	gl::BindBuffer(gl::ARRAY_BUFFER, colourBuffer);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * colourData.size(), &colourData[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VAO for triangle
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);
	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);

	//Bind VBOs to VAO
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
	gl::BindBuffer(gl::ARRAY_BUFFER, colourBuffer);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

	//Bind IBO to VAO
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);

	//perspective junk
	glm::mat4 scale = glm::mat4();
	glm::mat4 translate = glm::mat4();

	scale = glm::scale(modelView, glm::vec3(2.0f, 1.0f, 1.0f));
	//scale = glm::scale(modelView, glm::vec3(1.0f, 1.0f, 1.0f));
	translate = glm::translate(translate, glm::vec3(0.5f, 0.0f, -4.0f));
	//translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -1.0f));
	modelView =  scale*translate;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f,1.0f,0.0f));
	perspective = glm::perspective(glm::radians(45.f), (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);

	modelMatrixID = gl::GetUniformLocation(programHandle, "mModel");
	viewMatrixID = gl::GetUniformLocation(programHandle, "mView");
	projectionMatrixID =gl::GetUniformLocation(programHandle, "mProjection");

	gl::UniformMatrix4fv(modelMatrixID, 1, false, glm::value_ptr(modelView));
	gl::UniformMatrix4fv(viewMatrixID, 1, false, glm::value_ptr(view));
	gl::UniformMatrix4fv(projectionMatrixID, 1, false, glm::value_ptr(perspective));


	gl::BindVertexArray(0);

	indexSize = indices.size();
}

void SaltireScene::update(float t)
{
	angle++;
	if (angle>360.f)
	{
		angle -= 360;
	}
	glm::mat4 model = modelView;
	model = glm::translate(glm::vec3(0.0f, -0.5f, -4.0f)) * glm::rotate(glm::radians(angle), glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0.0f, 0.5f, 4.0f)) * model;
	

	gl::UniformMatrix4fv(modelMatrixID, 1, false, glm::value_ptr(model));
}

void SaltireScene::render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT);
	gl::BindVertexArray(vaoHandle);
	gl::DrawElements(gl::TRIANGLES, indexSize, gl::UNSIGNED_BYTE, BUFFER_OFFSET(0));
	gl::BindVertexArray(0);
}

void SaltireScene::resize(int, int)
{
}

void SaltireScene::linkMe(GLint vertShader, GLint fragShader)
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
