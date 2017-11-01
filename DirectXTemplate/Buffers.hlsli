
cbuffer Camera : register(b0)
{
    struct camera_t
    {
    
        float4x4 World;
        float4x4 View;
        float4x4 Projection;
    } Camera;
};
cbuffer Light : register(b1)
{
    struct light_t
    {
        float4 Position;
        float4 Color;
    } Light;
};
cbuffer Time : register(b2)
{
    struct time_t
    {
        uint Frame;
    } Time;
};