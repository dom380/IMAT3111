#include "RGBCubeScene.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

RGBCubeScene::RGBCubeScene()
{
	outlineEnabled = false;
}

RGBCubeScene::RGBCubeScene(bool outlineEnabled)
{
	this->outlineEnabled = outlineEnabled;
}

void RGBCubeScene::initScene()
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	//ifstream inFile("shader/basic.vert");
	ifstream inFile("shader/outline.vert");
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
	if (outlineEnabled) {
		fragFile = ifstream("shader/outline.frag");
	}
	else {
		fragFile = ifstream("shader/basic.frag");
	}
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
	//Basic triangle
	//float vertices[] = {
	//	-0.8f, -0.8f, 0.0f,
	//	0.8f, -0.8f, 0.0f,
	//	0.0f,  0.8f, 0.0f
	//};

	//GLubyte indices[] = {
	//	0,1,2
	//};

	//float colourData[] = {
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//};

	vector<float> vertices = {
		-0.8f, -0.8f, 0.0f, //0
		-0.8f, -0.4f, 0.0f, //1
		-0.4f, -0.8f, 0.0f,//2
		-0.4f, -0.4f, 0.0f,//3
		 0.0f, -0.4f, 0.0f,//4
		 0.0f, -0.8f, 0.0f,//5
		 0.4f, -0.4f, 0.0f,//6
		 0.4f, -0.8f, 0.0f,//7
		-0.4f, 0.0f, 0.0f,//8
		 0.0f, 0.0f, 0.0f,//9
		-0.4f, 0.4f, 0.0f,//10
		 0.0f, 0.4f, 0.0f,//11
		-0.4f, 0.8f, 0.0f,//12
		 0.0f, 0.8f, 0.0f//13
	};

	vector<int> barycentric = {
		1,0,0,
		0,0,1,
		0,1,0,
		1,0,0,
		0,1,0,
		0,0,1,
		0,0,1,
		1,0,0,
		0,0,1,
		1,0,0,
		0,1,0,
		0,0,1,
		1,0,0,
		0,1,0,
	};

	vector<GLubyte> indices = {
		0,1,2,
		1,3,2,
		2,3,5,
		3,4,5,
		5,4,7,
		4,6,7,
		3,8,4,
		8,9,4,
		8,10,9,
		10,11,9,
		10,12,11,
		12,13,11
	};

	vector<float> colourData = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
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

	//Create VBO for triangle Barycentric Coords
	GLuint barycentricBuffer;
	gl::GenBuffers(1, &barycentricBuffer);
	gl::BindBuffer(gl::ARRAY_BUFFER, barycentricBuffer);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(int) * barycentric.size(), &barycentric[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VAO for triangle
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);
	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);
	gl::EnableVertexAttribArray(2);

	//Bind VBOs to VAO
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
	gl::BindBuffer(gl::ARRAY_BUFFER, colourBuffer);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
	gl::BindBuffer(gl::ARRAY_BUFFER, barycentricBuffer);
	gl::VertexAttribPointer(2, 3, gl::INT, FALSE, 0, (GLubyte *)NULL);

	//Bind IBO to VAO
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);

	gl::BindVertexArray(0);

	indexSize = indices.size();

}

void RGBCubeScene::update(float t)
{
}

void RGBCubeScene::render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT);
	gl::BindVertexArray(vaoHandle);
	gl::DrawElements(gl::TRIANGLES, indexSize, gl::UNSIGNED_BYTE, BUFFER_OFFSET(0));
	gl::BindVertexArray(0);
}

void RGBCubeScene::resize(int w, int h)
{
	gl::Viewport(0, 0, w, h);
}

void RGBCubeScene::linkMe(GLint vertShader, GLint fragShader)
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
