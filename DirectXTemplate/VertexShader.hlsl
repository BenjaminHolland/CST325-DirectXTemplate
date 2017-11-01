#include "Structures.hlsli"
#include "Buffers.hlsli"
VS_TO_PS main(HOST_TO_VS input) 
{
    VS_TO_PS output;
    float4 pos = float4(input.Position.xyz, 1);
    pos = mul(Camera.World, pos);
    output.WorldPosition = pos;
    pos = mul(Camera.View, pos);

    pos = mul(Camera.Projection, pos);

    matrix w = Camera.World;
    w._14 = w._24=w._34=0;

    float4 norm;
    norm.xyz = input.Normal.xyz;
    norm.w = 1;
    norm = mul(w, norm);
    //norm.x *= -1;
    
    //TODO: Properly Transform Normals/Textures
    output.Position = pos;
    output.Texture = input.Texture;
    output.Color = input.Color;
    output.Normal = normalize(norm);

	return output;
}