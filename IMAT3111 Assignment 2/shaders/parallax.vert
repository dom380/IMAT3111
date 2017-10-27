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
   mat3 model = mat3(M);
   mat3 normMat = transpose(inverse(mat3(model))); 
   fragPos = vec3(model * VertexPosition);

   vec3 T = normalize(normMat  * VertexTangent.xyz) * VertexTangent.w;
   vec3 N = normalize(normMat * VertexNormal);
   T = normalize(T - dot(T, N) * N);
   vec3 B = cross(N,T);
   TBN = transpose(mat3(T,B,N));

   TangentlightPos = TBN * LightPosition; 
   TangentviewPos =  TBN * viewPos;
   TangentfragPos = TBN * fragPos;
   texCoords = VertexUV;
      
   gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
