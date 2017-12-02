#include <Shared.hlsli>

Texture2D Wave1 : register(t1);
Texture2D Wave2 : register(t2);

SamplerState DefaultSampler : register(s0);

VertexToPixel main(PipelineToVertex input)
{
    VertexToPixel output;
    float4 pos = float4(input.Position.xyz, 1);
    float2 dif = float2(TimeState.Discrete.x * 0.005, 0);
    float2 dif2 = float2(0, TimeState.Discrete.x * 0.005+1237);
    float h1 = Wave1.SampleLevel(DefaultSampler, input.Texture.xy +dif, 0).r;
    float h2 = Wave1.SampleLevel(DefaultSampler, input.Texture.xy + dif2, 0).r;
    float h = h1 + h2;
    pos.y += h;
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