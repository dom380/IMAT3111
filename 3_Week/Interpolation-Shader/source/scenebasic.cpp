#include "stdafx.h"

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

SceneBasic::SceneBasic() { }

void SceneBasic::initScene()
{
    //////////////////////////////////////////////////////
    /////////// Vertex shader //////////////////////////
    //////////////////////////////////////////////////////

    // Load contents of file
    ifstream inFile( "shader/basic.vert" );
    if (!inFile) {
        fprintf(stderr, "Error opening file: shader/basic.vert\n" );
        exit(1);
    }

    std::stringstream code;
    code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());

    // Create the shader object
    GLuint vertShader = gl::CreateShader( gl::VERTEX_SHADER );
    if (0 == vertShader) {
      fprintf(stderr, "Error creating vertex shader.\n");
      exit(EXIT_FAILURE);
    }

    // Load the source code into the shader object
    const GLchar* codeArray[] = {codeStr.c_str()};
    gl::ShaderSource(vertShader, 1, codeArray, NULL);

    // Compile the shader
    gl::CompileShader( vertShader );

    // Check compilation status
    GLint result;
    gl::GetShaderiv( vertShader, gl::COMPILE_STATUS, &result );
    if( FALSE == result ) {
       fprintf( stderr, "Vertex shader compilation failed!\n" );

       GLint logLen;
       gl::GetShaderiv( vertShader, gl::INFO_LOG_LENGTH, &logLen );

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
    ifstream fragFile( "shader/basic.frag" );
    if (!fragFile) {
        fprintf(stderr, "Error opening file: shader/basic.frag\n" );
        exit(1);
    }

    std::stringstream fragCode;
    fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

    // Create the shader object
    GLuint fragShader = gl::CreateShader( gl::FRAGMENT_SHADER );
    if (0 == fragShader) {
      fprintf(stderr, "Error creating fragment shader.\n");
      exit(1);
    }

    // Load the source code into the shader object
    codeArray[0] = codeStr.c_str();
    gl::ShaderSource( fragShader, 1, codeArray, NULL );

    // Compile the shader
    gl::CompileShader( fragShader );

    // Check compilation status
    gl::GetShaderiv( fragShader, gl::COMPILE_STATUS, &result );
    if (FALSE == result) {
       fprintf( stderr, "Fragment shader compilation failed!\n" );

       GLint logLen;
       gl::GetShaderiv( fragShader, gl::INFO_LOG_LENGTH, &logLen );

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
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };
    float colourData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };


#ifdef SINGLE_BUFFER
	// Create and set-up the vertex array object using an interlaced buffer
 gl::GenVertexArrays( 1, &vaoHandle );
    gl::BindVertexArray(vaoHandle);

	//Create and load buffer
	gl::GenBuffers(1,&vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER,vbo);
	gl::BufferData(gl::ARRAY_BUFFER,3*6*sizeof(GLfloat),NULL,gl::STATIC_DRAW);
	gl::BufferSubData(gl::ARRAY_BUFFER,0,3*3*sizeof(GLfloat),positionData);
	gl::BufferSubData(gl::ARRAY_BUFFER,3*3*sizeof(GLfloat),3*3*sizeof(GLfloat),colourData);


	//Get the VertexPosition attrib handle
	GLuint loc1;
	loc1 =gl::GetAttribLocation(programHandle,"VertexPosition");
	gl::EnableVertexAttribArray(loc1);
	//Set the location and tell it the data format
	gl::VertexAttribPointer(loc1,3,gl::FLOAT,FALSE,0,0);

    	//Get the VertexColour attrib handle
	GLuint loc2;
	loc2 =gl::GetAttribLocation(programHandle,"VertexColour");
	gl::EnableVertexAttribArray(loc2);
	//Set the location and tell it the data format
	gl::VertexAttribPointer(loc2,3,gl::FLOAT,FALSE,0,BUFFER_OFFSET(3*3*sizeof(GLfloat)));

#else

    // Create and populate the buffer objects using separate buffers
    GLuint vboHandles[2];
    gl::GenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
    gl::BufferData(gl::ARRAY_BUFFER, 9 * sizeof(float), positionData, gl::STATIC_DRAW);

    gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferHandle);
    gl::BufferData(gl::ARRAY_BUFFER, 9 * sizeof(float), colourData, gl::STATIC_DRAW);

    // Create and set-up the vertex array object
    gl::GenVertexArrays( 1, &vaoHandle );
    gl::BindVertexArray(vaoHandle);

    gl::EnableVertexAttribArray(0);  // Vertex position
    gl::EnableVertexAttribArray(1);  // Vertex color

    gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
    gl::VertexAttribPointer( 0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );

    gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferHandle);
    gl::VertexAttribPointer( 1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );

	GLbyte indices[] = {
		0,1,2
	};
	GLuint indexBuffer;
	gl::GenBuffers(1, &indexBuffer);
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indexBuffer);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLubyte), &indices, gl::STATIC_DRAW);

	gl::BindVertexArray(0);

#endif

}

void SceneBasic::linkMe(GLint vertShader, GLint fragShader)
{

    
    // Create the program object
    programHandle = gl::CreateProgram();
    if(0 == programHandle) {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }


    // Attach the shaders to the program object
    gl::AttachShader( programHandle, vertShader );
    gl::AttachShader( programHandle, fragShader );

    // Link the program
    gl::LinkProgram( programHandle );

    // Check for successful linking
    GLint status;
    gl::GetProgramiv( programHandle, gl::LINK_STATUS, &status );
    if (FALSE == status) {

        fprintf( stderr, "Failed to link shader program!\n" );

        GLint logLen;
        gl::GetProgramiv( programHandle, gl::INFO_LOG_LENGTH, &logLen );

        if (logLen > 0) {
            char * log = (char *)malloc(logLen);

            GLsizei written;
            gl::GetProgramInfoLog(programHandle, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);

            free(log);
        }
    } else {
        gl::UseProgram( programHandle );
    }
    
}

void SceneBasic::update( float t )
{

}

void SceneBasic::render()
{
    gl::Clear(gl::COLOR_BUFFER_BIT);

    gl::BindVertexArray(vaoHandle);
    //gl::DrawArrays(gl::TRIANGLES, 0, 3 );
	gl::DrawElements(gl::TRIANGLES, 3, gl::UNSIGNED_BYTE, (void*)0);

}

void SceneBasic::resize(int w, int h)
{
    gl::Viewport(0,0,w,h);
}
