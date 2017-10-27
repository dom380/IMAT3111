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
uniform bool postProcess;
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
	if(!postProcess)
	{
		FragColor = texture(RenderTex, TexCoord);
		return FragColor;
	}

	int intensityCout[50];
	double averageR[50];
	double averageG[50];
	double averageB[50];
    double dx = 1.0 / double(Width);
    double dy = 1.0 / double(Height);
	for(int i =0; i < 50; i++)
	{
		intensityCout[i] = 0;	
		averageR[i] = 0;
		averageG[i] = 0;
		averageB[i] = 0;
	}

	vec2 src_size = vec2(dx, dy);
	vec2 uv = TexCoord;

	vec3 colour;
	int curIntensityBin;

	for (int j = -radius; j <= 0; ++j)  {
         for (int i = -radius; i <= 0; ++i)  {
             colour = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			 curIntensityBin = int(((colour.r+colour.g+colour.b)/3.0)*intensityLevel);
			 curIntensityBin = min(curIntensityBin, intensityLevel-1);
			 intensityCout[curIntensityBin]++;
			 averageR[curIntensityBin] += colour.r;
			 averageG[curIntensityBin] += colour.g;
			 averageB[curIntensityBin] += colour.b;
         }
     }

     for (int j = -radius; j <= 0; ++j)  {
         for (int i = 0; i <= radius; ++i)  {
             colour = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			 curIntensityBin = int(((colour.r+colour.g+colour.b)/3.0)*intensityLevel);
			 curIntensityBin = min(curIntensityBin, intensityLevel-1);
			 intensityCout[curIntensityBin]++;
			 averageR[curIntensityBin] += colour.r;
			 averageG[curIntensityBin] += colour.g;
			 averageB[curIntensityBin] += colour.b;
         }
     }

     for (int j = 0; j <= radius; ++j)  {
         for (int i = 0; i <= radius; ++i)  {
             colour = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			 curIntensityBin = int(((colour.r+colour.g+colour.b)/3.0)*intensityLevel);
			 curIntensityBin = min(curIntensityBin, intensityLevel-1);
			 intensityCout[curIntensityBin]++;
			 averageR[curIntensityBin] += colour.r;
			 averageG[curIntensityBin] += colour.g;
			 averageB[curIntensityBin] += colour.b;
         }
     }

     for (int j = 0; j <= radius; ++j)  {
         for (int i = -radius; i <= 0; ++i)  {
             colour = texture(RenderTex, uv + vec2(i,j) * src_size).rgb;
			 curIntensityBin = int(((colour.r+colour.g+colour.b)/3.0)*intensityLevel);
			 curIntensityBin = min(curIntensityBin, intensityLevel-1);
			 intensityCout[curIntensityBin]++;
			 averageR[curIntensityBin] += colour.r;
			 averageG[curIntensityBin] += colour.g;
			 averageB[curIntensityBin] += colour.b;
         }
     }

	//return  texture(RenderTex, TexCoord);
	int maxIndex;
	int curMax = 0;
	for(int i =0; i < intensityLevel; ++i)
	{
		if(intensityCout[i] > curMax)
		{
			curMax = intensityCout[i];
			maxIndex = i;
		}
	}
	double finalR = clamp(averageR[maxIndex] / double(curMax), 0.0, 1.0);
	double finalG = clamp(averageG[maxIndex] / double(curMax), 0.0, 1.0);
	double finalB = clamp(averageB[maxIndex] / double(curMax), 0.0, 1.0);
	FragColor = vec4(finalR, finalG, finalB, 1.0);
	return FragColor;
}

void main()
{
    // This will call either pass1() or pass2()
    FragColor = RenderPass();
}
