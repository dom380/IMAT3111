#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColour;
layout (location=2) in vec3 barycentric;

out vec3 vBC;

out vec3 Colour;

void main()
{
    Colour = VertexColour;
	vBC = barycentric;
    gl_Position = vec4(VertexPosition,1.0);
}
