#version 430
in vec3 N;					//Normal
in vec3 Position;			//Vertex position in world space

layout( location = 0 ) out vec4 FragColour;

uniform samplerCube skybox; //Cube map
uniform vec3 camPos; //Camera position in world space
uniform bool refraction; //toggle reflection or refraction

void main() {
	vec3 I = normalize(Position - camPos);
	vec3 R;
	if(refraction){ //If refraction enable
		R = refract(I, normalize(N), 1.0/1.52);
	} else {
		R = reflect(I, normalize(N));
	}
   
   FragColour = texture(skybox, R);
}
