#version 430

in vec3 TangentlightPos; //Light position
in vec3 TangentfragPos;
in vec3 TangentviewPos;
in vec3 fragPos; //Fragment positions
in vec2 texCoords;
in mat3 TBN;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform vec3 Ks;			// Specular reflection coefficent 
uniform float n;			// specular exponent	
uniform vec3 Ia;			// Ambient intensity

layout( location = 0 ) out vec4 FragColour;

vec4 calcShading(vec4 normal, vec4 textureColour);

void main() {
	

	vec4 normal = vec4(texture(normalTexture, texCoords).rgb, 1.0);
	// Transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	vec4 texture = texture(diffuseTexture, texCoords);

	FragColour = calcShading(normal, texture);
}


vec4 calcShading(vec4 normal, vec4 textureColour)
{
	//ambient
	vec4 ambient = vec4(Ia * Kd,1.0);
	ambient = clamp(ambient, 0.0, 1.0);
	//diffuse
	vec4 lightDir = vec4(normalize(TangentlightPos - TangentfragPos), 1.0);
	vec4 Id = vec4(Ld,1.0) * max(dot(normal,lightDir), 0.0) * textureColour;
	Id = clamp(Id, 0.0, 1.0); 

	//specular
	vec3 viewDir = normalize(TangentviewPos - TangentfragPos);
	vec3 R = vec3(-reflect(lightDir, normal));
	float Is = pow(max(dot(R,viewDir), 0.0),n);
	Is = clamp(Is, 0.0, 1.0);
	return ambient  + vec4(Ld,1.0)*( (vec4(Kd,1.0) * Id) + (vec4(Ks,1.0) * Is));
}