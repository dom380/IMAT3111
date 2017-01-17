#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 N; //Transformed normal
out vec3 lightPos; //Light position in eye coords
out vec3 viewPos; //Viewport position in eye coords
out vec3 fragPos; //Fragment position in model coords



uniform vec3 LightPosition; // Light position 
uniform vec3 lightTarget;
uniform mat3 NormalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
   lightPos = vec3(M * vec4(LightPosition,1.0));  

   fragPos = vec3(M * vec4(VertexPosition, 1.0f));

   viewPos = -vec3(V * M * vec4(VertexPosition,1.0)); 

   N = normalize( NormalMatrix * VertexNormal);
      
   gl_Position = gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
