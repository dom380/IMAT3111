#version 430

// The Light information
struct LightInfo {
  vec4 Position;  // Light position in eye coords. Actually no its not. It never was and would never work. - Dom
  vec3 AmbientIntensity; 
  vec3 DiffuseIntensity;
  float linear;
  float quadratic;
};
uniform LightInfo Light;

// The Material information
struct MaterialInfo {
  vec3 Kd;            // Ambient and Diffuse reflectivity
  vec3 Ks;
  float spec;
};
uniform MaterialInfo Material;

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

// The textures produced by the pass1
uniform sampler2D PositionTex, NormalTex, ColorTex, SpecularTex; 
uniform vec3 viewPos;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData; //COLOR_ATTACHMENT0
layout (location = 2) out vec3 NormalData; //COLOR_ATTACHMENT1
layout (location = 3) out vec4 ColorData; //COLOR_ATTACHMENT2
layout (location = 4) out vec4 SpecularData; //COLOR_ATTACHMENT3


//Calculate the fragment colour under ambient + diffuse light
//using position, normal and diffuse colour information sampled from
//textures
vec3 diffuseModel( vec3 pos, vec3 norm, vec3 diff )
{
    vec3 s = normalize(vec3(Light.Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = clamp(Light.AmbientIntensity * diff + Light.DiffuseIntensity 
* diff * sDotN,0,1);

    return diffuse;
}

vec3 calcSpecular(vec3 pos, vec3 norm, vec3 lightDir, vec4 spec)
{
	vec3 viewDir = normalize(viewPos - pos);
	vec3 reflect = normalize(-reflect(lightDir, norm));
	vec3 specular =  Light.DiffuseIntensity * pow(max(dot(viewDir, reflect), 0.0), spec.a*255) * spec.rgb;
	return specular;
}

float calcFatt(float distance)
{
	float attenuation = 1.0f / (1.0 + Light.linear * distance + 
    				Light.quadratic * (distance * distance));    
	return attenuation; 
}

vec3 phongModel(vec3 pos, vec3 norm, vec4 diff, vec4 spec)
{
	vec3 lightDir = vec3(Light.Position) - pos;
	vec3 s = normalize(lightDir);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = clamp(Light.DiffuseIntensity * diff.rgb * sDotN,0,1);
	vec3 ambient = Light.AmbientIntensity * diff.rgb;
	vec3 specular =  calcSpecular(pos, norm, s, spec);
	float attenuation = calcFatt(length(lightDir));
	return clamp(((ambient + diffuse + specular)*attenuation), 0, 1);

}

//pass1
subroutine (RenderPassType)
void pass1()
{
    // Store position, normal, and diffuse color in textures
    PositionData = Position;
    NormalData = Normal;
    ColorData.rgb = Material.Kd;
	if( Material.spec > 0){
		SpecularData.a = Material.spec/255;
		SpecularData.rgb = Material.Ks;
	} else {
		SpecularData = vec4(0.0,0.0,0.0,1.0);
	}
}

//pass2
subroutine(RenderPassType)
void pass2()
{
	vec3 pos = vec3(texture(PositionTex, TexCoord));
	vec3 norm = vec3(texture(NormalTex, TexCoord));
	vec4 diffColor = vec4(texture(ColorTex, TexCoord)); 
	vec4 specular = vec4(texture(SpecularTex, TexCoord));
	//FragColor = vec4(calcSpecular(pos, norm, normalize(vec3(Light.Position) - pos), specular),1.0);
	//FragColor = vec4(pos,1.0);
	//FragColor = vec4(norm,1.0);
	//FragColor = diffColor; //Albedo
	//FragColor = vec4(diffuseModel(pos, norm, diffColor.rgb), 1.0);
	FragColor = vec4(phongModel(pos, norm, diffColor, specular), 1.0);
}

void main() {
    // This will call either pass1 or pass2
    RenderPass();
}
