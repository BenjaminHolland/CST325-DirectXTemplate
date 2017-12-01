#include <Shared.hlsli>
Texture2D SkyboxTexture : register(t0);
SamplerState DefaultSampler : register(s0);
float4 main(VertexToPixel input) : SV_TARGET
{
    return SkyboxTexture.Sample(DefaultSampler, input.Texture);
}