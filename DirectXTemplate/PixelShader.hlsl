#include "Structures.hlsli"
#include "Buffers.hlsli"
float4 main(VS_TO_PS input):SV_TARGET
{
    float4 ldir = input.WorldPosition-Light.Position ;
    float4 ldir_n = normalize(ldir);
    float len = length(ldir);
    float lang = dot(input.Normal,ldir_n );
    float coef_dif = saturate(lang)/len;
    float3 color_dif = coef_dif*float3(input.Texture.x, input.Texture.y, 0);
    
    return float4(color_dif, 1);
}