#version 430

in vec3 TangentlightPos; //Light position
in vec3 TangentfragPos;
in vec3 TangentviewPos;
in vec3 fragPos; //Fragment positions
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

vec4 calcShading(vec3 viewDir, vec4 normal, vec4 textureColour, vec3 spec);
vec2 parallaxMap(vec2 texCoords, vec3 view);
vec2 parallaxMapSteep(vec2 texCoords, vec3 view);


void main() {
	
	vec3 viewDir = normalize(TangentviewPos - TangentfragPos);
	//vec2 textureCoords = parallaxMap(texCoords, viewDir);
	vec2 textureCoords = parallaxMapSteep(texCoords, viewDir);
	if(textureCoords.x > 1.0 || textureCoords.y > 1.0 || textureCoords.x < 0.0 || textureCoords.y < 0.0)
		discard;
	vec4 normal = vec4(texture(normalTexture, textureCoords).rgb, 1.0); // this normal is in tangent space
	normal = normalize((normal * 2.0) - 1.0); // Transform normal vector to range [-1,1]
	vec4 textureColour = texture(diffuseTexture, textureCoords);
	vec3 specColour = vec3(texture(specularTexture, textureCoords));
	FragColour = calcShading(viewDir, normal, textureColour, specColour);
}


vec4 calcShading(vec3 viewDir, vec4 normal, vec4 textureColour, vec3 spec)
{
	//ambient
	vec4 ambient = vec4(Ia * Kd,1.0) * textureColour;
	ambient = clamp(ambient, 0.0, 1.0);

	//diffuse
	vec4 lightDir = vec4(normalize(TangentlightPos - TangentfragPos), 1.0);
	float diff = max(dot(lightDir, normal), 0.0);
	vec4 diffuse = clamp(diff * vec4(Ld,1.0) * textureColour, 0.0, 1.0);

	//specular
	vec3 reflectDir = vec3(-reflect(lightDir, normal));
	vec4 specular =  clamp(vec4(Ld,1.0) * pow(max(dot(viewDir, reflectDir), 0.0), matSpec) * vec4(spec, 1.0), 0.0, 1.0);


	return (ambient + diffuse + specular);
}

vec2 parallaxMap(vec2 tex, vec3 view)
{
	float height = texture(depthTexture, tex).r;
	return tex + (view.xy / view.z * (height * heightScale)); 
}

vec2 parallaxMapSteep(vec2 texCoords, vec3 view)
{
	// number of depth layers
    const float minLayers = 10;
    const float maxLayers = 25;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), view)));  //This allows us to lower the number of samples if facing the camera
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = view.xy / view.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthTexture, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue) //Keep iterating down the layers until we reach a a depth value along the view dir that's below the depth map value.
    {
        // shift texture coordinates along direction of P
        currentTexCoords += deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthTexture, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
 
	vec2 prevTexCoords = currentTexCoords - deltaTexCoords; //Then get the previous layers texture coordinate

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(depthTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;  
}