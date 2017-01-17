#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 N;			//Transformed normal
out vec3 Position;  //Vertex position in model space

uniform mat3 NormalMatrix; // Normal Matrix (Tranpose of inverse of model matrix)
uniform mat4 M;	//Model Matrix
uniform mat4 V; //View matrix
uniform mat4 P; //Projection Matrix

void main()
{

   N = normalize( NormalMatrix * VertexNormal);

   Position = vec3(M * vec4(VertexPosition,1.0));
      
   gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
