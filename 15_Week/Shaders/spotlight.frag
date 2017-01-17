#version 430

in vec3 N;
in vec3 lightPos;
in vec3 viewPos;			//Direction to view port
in vec3 fragPos;
in vec3 lightPosM;



uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform vec3 Ks;			// Specular reflection coefficent 
uniform float n;			// specular exponent	
uniform vec3 Ia;			// Ambient intensity	
uniform float a;			// Attenuation distance
uniform float f;			// Attenuation falloff

uniform float cutoffAngle;
uniform float outerCutoffAngle;
uniform vec3 targetDirection;

layout( location = 0 ) out vec4 FragColour;

void main() {

	//Ambient
	vec4 ambient = vec4(Ia * Kd,1.0);
	ambient = clamp(ambient, 0.0, 1.0);

   //Calculate the light vector
   vec3 lightDirection = normalize(lightPos - fragPos);  
   vec3 targetDir = normalize(-targetDirection);
   float theta = dot(lightDirection, targetDir);
   float epsilon = (cutoffAngle - outerCutoffAngle);

	//calculate Fatt
	float d = length(lightPos - fragPos);
	float fatt = 1 - pow(d/a,f);

	float spotAttenuation = clamp((theta - outerCutoffAngle)/epsilon, 0.0, 1.0);

	//calculate Diffuse Light Intensity making sure it is not negative 
	//and is clamped 0 to 1  
	vec4 Id = vec4(Ld,1.0) * max(dot(N,lightDirection), 0.0);
	Id = clamp(Id, 0.0, 1.0);     

	//Calculate specular
	vec3 viewDir = normalize(viewPos);
	vec3 R = -reflect(lightDirection,N);
	float Is = pow(max(dot(R,viewDir), 0.0),n);
	Is = clamp(Is, 0.0, 1.0);
  
	//Ambient + (Light intensity * Fatt * (diffuse + specular)) * spotAttenuation
	FragColour = ambient  + (vec4(Ld,1.0)*fatt*( (vec4(Kd,1.0) * Id) + (vec4(Ks,1.0) * Is))) * spotAttenuation;

}
