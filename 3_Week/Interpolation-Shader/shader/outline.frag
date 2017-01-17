

#version 430

in vec3 Colour;
in vec3 vBC;
layout (location=0) out vec4 FragColour;

void main() {
    if(any(lessThan(vBC, vec3(0.02)))){
		FragColour = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else{
		FragColour = vec4(Colour,1.0);
	}
}
