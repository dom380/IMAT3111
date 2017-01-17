#version 430

in vec3 texCoord;
uniform samplerCube skybox; //Cube map
layout( location = 0 ) out vec4 FragColour;

void main() {
   FragColour = texture(skybox, texCoord);
}
