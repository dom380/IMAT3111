#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;
layout (location = 3) in vec4 VertexTangent;


out vec3 TangentlightPos; //Light position
out vec3 TangentviewPos;
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
	vec4 worldPosition = M * vec4(VertexPosition,1.0);
	vec3 worldNormal = normalize(NormalMatrix * VertexNormal);
	vec3 worldTangent = normalize(NormalMatrix * VertexTangent.xyz);

	vec3 worldDirToLight = normalize(LightPosition - worldPosition.xyz);
	vec3 worldDirToCamera = normalize(viewPos - worldPosition.xyz);
	vec3 worldBitanget = cross(worldNormal, worldTangent) * VertexTangent.w;

	TangentlightPos = vec3(
			dot(worldDirToLight, worldTangent),
			dot(worldDirToLight, worldBitanget),
			dot(worldDirToLight, worldNormal)
		);
	TangentviewPos = vec3(
			dot(worldDirToCamera, worldTangent),
			dot(worldDirToCamera, worldBitanget),
			dot(worldDirToCamera, worldNormal)
		);
	texCoords = VertexUV;
	gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
