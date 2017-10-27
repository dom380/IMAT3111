#version 430

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform sampler2DShadow ShadowMap;
uniform  vec3 LightIntensity;

in vec3 Position;
in vec3 Normal;
in vec4 ShadowCoord;
in vec3 LightPos;


layout (location = 0) out vec4 FragColor;

vec3 phongModelDiffAndSpec()
{
    vec3 n = Normal;
   if( !gl_FrontFacing ) n = -n;
    vec3 s = normalize(vec3(LightPos) - Position);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = LightIntensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = LightIntensity * Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return diffuse + spec;
}

float calculateShadow(vec4 shadowCoord)
{
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    float currentDepth = projCoords.z + 0.0001;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - Position);
	float cosTheta = dot(normal, lightDir);
    float bias = 0.05*tan(acos(cosTheta));
	//bias = clamp(bias, 0.0, 0.01);
    float shadow = textureProj(ShadowMap, shadowCoord);
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
	float sampleCount = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
			sampleCount++;
            float pcfDepth = textureProjOffset(ShadowMap, shadowCoord, ivec2(x,y));
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= sampleCount;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine (RenderPassType)
void shadeWithShadow()
{
	vec3 ambient = Material.Ka * LightIntensity;
	vec3 color = phongModelDiffAndSpec();
	float shadow = calculateShadow(ShadowCoord);
	vec3 lighting = (ambient + ((1.0-shadow) * color));
	FragColor = vec4(lighting, 1.0);
}



subroutine (RenderPassType)
void recordDepth()
{
    // Outputs gl_FragCoord.z but no code needed as depth is written automatically by Frag Shaders

}

void main() {
    // This will call either shadeWithShadow or recordDepth
    RenderPass();
}
