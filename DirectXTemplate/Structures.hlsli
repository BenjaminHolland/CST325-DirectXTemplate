struct HOST_TO_VS
{
    float4 Position : POSITION;
    float4 Color : COLOR0;
    float4 Normal : NORMAL0;
    float2 Texture : TEXTURE0;
};
struct VS_TO_PS
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION1;
    float4 Color : COLOR0;
    float4 Normal : NORMAL0;
    float2 Texture : TEXTURE0;
};