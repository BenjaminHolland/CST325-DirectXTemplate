#include <Shared.hlsli>
Texture2D HeightMap : register(t0);
SamplerState DefaultSampler : register(s0);

VertexToPixel main(PipelineToVertex input)
{
    VertexToPixel output;
    float4 pos = float4(input.Position.xyz, 1);
    float2 dif = float2(TimeState.Discrete.x * 0.01, 0);
    float r = HeightMap.SampleLevel(DefaultSampler, input.Texture.xy +dif, 0).r;
    pos.y += r*2;
    pos = mul(TransformState.World, pos);
    output.WorldPosition = pos;
    pos = mul(TransformState.View, pos);
    pos = mul(TransformState.Projection, pos);
    output.Position = pos;

    output.Color = input.Color;
    
    float4 normal = float4(input.Normal, 1);
    float4x4 world = TransformState.World;
    world._14 = world._24 = world._34 = 0;
    normal = mul(normal, world);
    output.Normal = normalize(normal.xyz);

   // output.Normal = input.Normal;
    output.Texture = input.Texture;
    return output;
}