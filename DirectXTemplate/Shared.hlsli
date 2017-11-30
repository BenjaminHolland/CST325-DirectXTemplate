
cbuffer TransformBuffer : register(b0)
{
    struct TransformStateBufferType
    {
        float4x4 World;
        float4x4 View;
        float4x4 Projection;
    } TransformState;
}
cbuffer CameraBuffer : register(b1)
{
    struct CameraStateBufferType
    {
        float3 Position;
        float3 Target;
        float2 pad;
    } CameraState;
}
cbuffer TimeBuffer : register(b2)
{
    struct TimeStateBufferType
    {
        uint2 Discrete;
        float2 Continuious;
    } TimeState;
}
struct PipelineToVertex
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float4 Color : COLOR0;
    float2 Texture : TEXCOORD0;
};
struct VertexToPixel
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float3 Normal : NORMAL0;
    float4 Color : COLOR0;
    float2 Texture : TEXCOORD0;
};