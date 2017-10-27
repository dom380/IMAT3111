#version 430

in vec3 TangentlightPos; //Light position
in vec3 TangentviewPos;
in vec2 texCoords;
in mat3 TBN;

//Textures
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D depthTexture;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform vec3 Ks;			// Specular reflection coefficent 	
uniform float matSpec;		// Material shininess 
uniform vec3 Ia;			// Ambient intensity

uniform float heightScale;

layout( location = 0 ) out vec4 FragColour;

vec4 calcShading(vec2 tex, vec3 light, vec3 view);
vec2 parallaxMap(vec2 texCoords, vec3 view);

void main() {
	
	vec3 viewDir = normalize(TangentviewPos);
	vec3 lightDir = normalize(TangentlightPos);

	vec2 textureCoords = parallaxMap(texCoords, viewDir);
	if(textureCoords.x > 1.0 || textureCoords.y > 1.0 || textureCoords.x < 0.0 || textureCoords.y < 0.0)
		discard;

	FragColour = calcShading(textureCoords, lightDir, viewDir);
}


vec4 calcShading(vec2 tex, vec3 light, vec3 view)
{
	vec4 textureColour = texture(diffuseTexture, tex);
	vec3 normal =  vec3(texture(normalTexture, tex));

	//ambient
	vec4 ambient = vec4(Ia * Kd,1.0) * textureColour;
	ambient = clamp(ambient, 0.0, 1.0);

	//diffuse
	float diff = max(dot(light, normal), 0.0);
	vec4 diffuse = clamp(diff * vec4(Ld,1.0) * textureColour, 0.0, 1.0);

	//specular
	vec4 spec = texture(specularTexture, tex);
	vec3 reflectDir = vec3(reflect(-light, normal));
	vec4 specular =  clamp(vec4(Ld,1.0) * pow(max(dot(view, reflectDir), 0.0), matSpec) * spec, 0.0, 1.0);


	return (ambient + diffuse + specular);
}

vec2 parallaxMap(vec2 tex, vec3 view)
{
	float height = texture(depthTexture, tex).r;
	vec2 texOffset = heightScale * view.xy / view.z * height;
	texOffset = heightScale * view.xy * height;
	return tex - texOffset;
}