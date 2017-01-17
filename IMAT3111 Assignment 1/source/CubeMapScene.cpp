#include "CubeMapScene.h"

void CubeMapScene::initScene(QuatCamera camera) {
	//|Compile and link the shader  
	compileAndLinkShader();

	gl::Enable(gl::DEPTH_TEST);

	//A matrix to move the teapot lid upwards
	glm::mat4 lid = glm::mat4(1.0);
	//lid *= glm::translate(vec3(0.0,1.0,0.0));

	//Create the teapot with translated lid
	teapot = new VBOTeapot(16, lid);

	buildSkybox();

	m_showCubeMap = true;
}

void CubeMapScene::render(QuatCamera camera) {
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	gl::BindTexture(gl::TEXTURE_CUBE_MAP, textureID); //Bind cube map texture
	
	teapotShader.use(); //Bind teapot shader
	teapotShader.setUniform("refraction", m_refract); //set the refraction flag to true or false
	model = mat4(1.0f);
	setMatrices(camera, &teapotShader);
	teapot->render();
	if (m_showCubeMap) { //If should render skybox.
		skyboxShader.use(); //Bind skybox shader
		model = mat4(1.0f);
		model = glm::scale(model, glm::vec3(100, 100, 100)); //Scale unit cube to 100x100x100
		setMatrices(camera, &skyboxShader);
		gl::BindVertexArray(vaoHandle);
		gl::DrawElements(gl::TRIANGLES, indexSize, gl::UNSIGNED_SHORT, ((GLushort *)NULL + (0)));
	}

	gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);
}

void CubeMapScene::resize(QuatCamera camera, int w, int h) {
	gl::Viewport(0, 0, w, h);
	width = w;
	height = h;
	camera.setAspectRatio((float)w / h);
}

void CubeMapScene::setMatrices(QuatCamera camera, GLSLProgram* shader)
{
	mat4 mv = camera.view() * model;
	// the correct matrix to transform the normal is the transpose of the inverse of the M matrix
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	shader->setUniform("M", model);
	shader->setUniform("NormalMatrix", normMat);
	shader->setUniform("V", camera.view());
	shader->setUniform("P", camera.projection());
	shader->setUniform("camPos", camera.position());
}

void CubeMapScene::compileAndLinkShader()
{
	try {
		teapotShader.compileShader("Shaders/teapot.vert");
		teapotShader.compileShader("Shaders/teapot.frag");
		teapotShader.link();
		teapotShader.validate();

		skyboxShader.compileShader("Shaders/skybox.vert");
		skyboxShader.compileShader("Shaders/skybox.frag");
		skyboxShader.link();
		skyboxShader.validate();

		teapotShader.use();
	}
	catch (GLSLProgramException & e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format)
{
	switch (format) {
	case Bitmap::Format_RGB: return gl::RGB;
	case Bitmap::Format_RGBA: return gl::RGBA;
	default: throw std::runtime_error("Unrecognised Bitmap::Format");
	}
}

void CubeMapScene::buildSkybox()
{
	skyboxVertices = {
		1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
		1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
		1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
		-1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
		-1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
		1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1
	};
	vector<GLushort> skyboxIndices = {
		0, 1, 2,   2, 3, 0,      // front
		4, 5, 6,   6, 7, 4,      // right
		8, 9,10,  10,11, 8,      // top
		12,13,14,  14,15,12,      // left
		16,17,18,  18,19,16,      // bottom
		20,21,22,  22,23,20
	};

	GLuint vboHandles[2];
	gl::GenBuffers(2, vboHandles);
	//Create IBO for triangle
	GLuint indiceBuffer = vboHandles[0];
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * skyboxIndices.size(), &skyboxIndices[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);

	//Create VBO for triangle vertices
	GLuint vertexBuffer = vboHandles[1];
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * skyboxVertices.size(), &skyboxVertices[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	//Create VAO for triangle
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);
	gl::EnableVertexAttribArray(0);

	//Bind VBOs to VAO
	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBuffer);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLushort *)NULL);

	//Bind IBO to VAO
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indiceBuffer);
	indexSize = static_cast<GLuint>(skyboxIndices.size());

	//Load cube map textures
	gl::GenTextures(1, &textureID); //Generate a TextureID
	gl::ActiveTexture(gl::TEXTURE0); //Activate TEXTURE0
	vector<Bitmap> textures; //Load the 6 images into a vector
	textures.push_back(Bitmap::bitmapFromFile("textures/right.jpg"));
	textures.push_back(Bitmap::bitmapFromFile("textures/left.jpg"));
	textures.push_back(Bitmap::bitmapFromFile("textures/up.jpg"));
	textures.push_back(Bitmap::bitmapFromFile("textures/down.jpg"));
	textures.push_back(Bitmap::bitmapFromFile("textures/back.jpg"));
	textures.push_back(Bitmap::bitmapFromFile("textures/front.jpg"));

	gl::BindTexture(gl::TEXTURE_CUBE_MAP, textureID); //Bind cube map
	for (GLuint i = 0; i < 6; i++) //Set up texture parameters for all 6 faces of cube map
	{
	
		gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, //Face Enums stored in interative order (x+,x-,y+,y-,z+,z-)
			0,
			TextureFormatForBitmapFormat(textures.at(i).format()),
			(GLsizei)textures.at(i).width(),
			(GLsizei)textures.at(i).height(),
			0,
			TextureFormatForBitmapFormat(textures.at(i).format()),
			gl::UNSIGNED_BYTE,
			textures.at(i).pixelBuffer());
	}
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR); //Set up filter and wrap parameters
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);
	teapotShader.setUniform("skybox", 0);//set samplerCube to texture 0
	skyboxShader.setUniform("skybox", 0);//set samplerCube to texture 0
}
