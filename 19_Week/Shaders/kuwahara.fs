#version 430

in vec3 Position;
in vec3 N;
in vec2 TexCoord;

uniform sampler2D RenderTex;
uniform sampler2D diffuseTexture;

uniform float EdgeThreshold;
uniform int Width;
uniform int Height;
uniform int radius;
uniform int intensityLevel;
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

vec4 phongModel( vec3 pos, vec3 norm )
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

    return vec4((Ia + Id), 1.0) * texture(diffuseTexture, TexCoord);
}

subroutine (RenderPassType)
vec4 pass1()
{
   return phongModel( Position, N );
}

float luminance( vec3 color ) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

double dist(double dx, double dy)
{
    return sqrt(dx*dx + dy*dy);
}

subroutine( RenderPassType )
vec4 pass2()
{
    double dx = 1.0 / double(Width);
    double dy = 1.0 / double(Height);
	float n = float((radius + 1) * (radius + 1));
	vec2 src_size = vec2(dx, dy);
	vec2 uv = TexCoord;//vec2(TexCoord.x/double(Width), TexCoord.y/double(Height));
	vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0);
    vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0);
    vec3 c;
	int curIntensityBin;

	for (int j = -radius; j <= 0; ++j)  {
         for (int i = -radius; i <= 0; ++i)  {
             c = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			 m0 += c;
             s0 += c * c;
         }
     }

     for (int j = -radius; j <= 0; ++j)  {
         for (int i = 0; i <= radius; ++i)  {
             c = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			  m1 += c;
             s1 += c * c;
         }
     }

     for (int j = 0; j <= radius; ++j)  {
         for (int i = 0; i <= radius; ++i)  {
             c = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			   m2 += c;
             s2 += c * c;
         }
     }

     for (int j = 0; j <= radius; ++j)  {
         for (int i = -radius; i <= 0; ++i)  {
             c = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			  m3 += c;
             s3 += c * c;
         }
     }
	float min_sigma2 = 1e+2;
     m0 /= n;
     s0 = abs(s0 / n - m0 * m0);

     float sigma2 = s0.r + s0.g + s0.b;
     if (sigma2 < min_sigma2) {
         min_sigma2 = sigma2;
         FragColor = vec4(m0, 1.0);
     }

     m1 /= n;
     s1 = abs(s1 / n - m1 * m1);

     sigma2 = s1.r + s1.g + s1.b;
     if (sigma2 < min_sigma2) {
         min_sigma2 = sigma2;
         FragColor = vec4(m1, 1.0);
     }

     m2 /= n;
     s2 = abs(s2 / n - m2 * m2);

     sigma2 = s2.r + s2.g + s2.b;
     if (sigma2 < min_sigma2) {
         min_sigma2 = sigma2;
         FragColor = vec4(m2, 1.0);
     }

     m3 /= n;
     s3 = abs(s3 / n - m3 * m3);

     sigma2 = s3.r + s3.g + s3.b;
     if (sigma2 < min_sigma2) {
         min_sigma2 = sigma2;
         FragColor = vec4(m3, 1.0);
     }
	 return FragColor;
}

void main()
{
    // This will call either pass1() or pass2()
    FragColor = RenderPass();
}
