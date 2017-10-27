#version 430

in vec3 Position;
in vec3 N;
in vec2 TexCoord;

uniform sampler2D RenderTex;

uniform float EdgeThreshold;
uniform int Width;
uniform int Height;

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel( vec3 pos, vec3 norm )
{
    //Calculate the light vector
   vec3 L = normalize(vec3(Light.Position) - pos); 
 
   //Calculate View vector
   vec3 V = normalize(-pos); 	//We are in eye coords 
						//so camera is at (0,0,0); 
    

   //calculate Diffuse Light Intensity making sure it is not negative 
   //and is clamped 0 to 1  
     
   vec3 Id = Light.Intensity * max(dot(norm,L), 0.0);
   Id = clamp(Id, 0.0, 1.0);     
   
   vec3 Ia = Light.Intensity * Material.Ka;

    return Ia + Id;
}

subroutine (RenderPassType)
vec4 pass1()
{
   return vec4(phongModel( Position, N ),1.0);
}

float luminance( vec3 color ) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

subroutine( RenderPassType )
vec4 pass2()
{
    float dx = 1.0 / float(Width);
    float dy = 1.0 / float(Height);

    float s00 = luminance(texture( RenderTex, TexCoord 
								+ vec2(-dx,dy) ).rgb);
    float s10 = luminance(texture( RenderTex, TexCoord 
								+ vec2(-dx,0.0) ).rgb);
    float s20 = luminance(texture( RenderTex, TexCoord 
								+ vec2(-dx,-dy) ).rgb);
    float s01 = luminance(texture( RenderTex, TexCoord 
								+ vec2(0.0,dy) ).rgb);
    float s21 = luminance(texture( RenderTex, TexCoord 
								+ vec2(0.0,-dy) ).rgb);
    float s02 = luminance(texture( RenderTex, TexCoord 
								+ vec2(dx, dy) ).rgb);
    float s12 = luminance(texture( RenderTex, TexCoord 
								+ vec2(dx, 0.0) ).rgb);
    float s22 = luminance(texture( RenderTex, TexCoord 
								+ vec2(dx, -dy) ).rgb);

    float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
    float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);

    float dist = sx * sx + sy * sy;

    if( dist > EdgeThreshold )
        return vec4(1.0); // White pixel
    else
        return vec4(0.0,0.0,0.0,1.0); // Black pixel
}

void main()
{
    // This will call either pass1() or pass2()
    FragColor = RenderPass();
}
