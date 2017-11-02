#include "Structures.hlsli"
#include "Buffers.hlsli"

float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float noise(float3 x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f

    float3 p = floor(x);
    float3 f = frac(x);

    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
                   lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
               lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
                   lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}
float4 main(VS_TO_PS input):SV_TARGET
{
    float4 ldir = Light.Position - input.WorldPosition;
    float ldist = length(ldir);
    float4 ldir_n = normalize(ldir);
    
    float4 cdir = Camera.Position - input.WorldPosition;
    float cdist = length(cdir);
    float4 cdir_n = normalize(cdir);
    
    float4 surf_n = normalize(input.Normal);
    
    float2 n1 = input.Texture + float2(100, 50);
    float2 n2 = input.Texture + float2(50, 100);
    float2 noise_n = float2(abs(noise(float3(n1 * 10, 0))), abs(noise(float3(n2 * 10, 0))));
    float3 cross_vector = normalize(cross(surf_n.xyz, float3(1, 1, 1)));
    float h = cos(0.05);
    
    float lang = dot(surf_n,ldir_n );

    float phi = 2 * 3.14159 * noise_n.x;
    float z = h + (1 - h) * noise_n.y;

    float sinT = sqrt(1 - z * z);
    float x = cos(phi) * sinT;
    float y = sin(phi) * sinT;
   
    //surf_n = normalize(surf_n);

    surf_n.xyz = float3(1, 1, 1) * x + cross_vector * y + surf_n.xyz * z;
    
    float4 H = normalize(cdir_n + ldir_n);
    float NdotH = dot(surf_n, H);
    float intensity = pow(saturate(NdotH), 3);
    float coef_dif = lang;
    float coef_spc = intensity * 100/ldist;
    float3 color_tex = DefaultTexture.Sample(DefaultSampler, input.Texture);
   // float3 color_dif = surf_n;
    float3 color_dif = 0.1 + saturate(coef_spc * coef_dif * color_tex );
    return float4(color_dif, 1);
}