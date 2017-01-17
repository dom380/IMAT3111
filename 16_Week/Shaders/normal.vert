#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;
layout (location = 3) in vec4 VertexTangent;


out vec3 TangentlightPos; //Light position
out vec3 TangentfragPos;
out vec3 TangentviewPos;
out vec3 fragPos; //Fragment positions
out vec2 texCoords;
out mat3 TBN;

uniform vec3 viewPos;
uniform vec3 LightPosition; // Light position 
uniform mat3 NormalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
   vec3 T = normalize(vec3(M * VertexTangent));
   vec3 N = normalize( vec3(NormalMatrix * VertexNormal));
   vec4  TcrossN = vec4(cross(T,N), 1.0);
   vec3 B = normalize(vec3(M * TcrossN));
   TBN = transpose(mat3(T,B,N));
   TangentlightPos = TBN * LightPosition; //vec3(M * vec4(LightPosition,1.0));  
   TangentviewPos =  TBN * viewPos; //-vec3(V * M * vec4(VertexPosition,1.0)); 
   fragPos = vec3(M * vec4(VertexPosition, 1.0f));
   TangentfragPos = TBN * fragPos;
   texCoords = VertexUV;
      
   gl_Position = gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
