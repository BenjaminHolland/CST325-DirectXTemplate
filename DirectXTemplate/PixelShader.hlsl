#include <Shared.hlsli>
Texture2D Noise1 : register(t0);
SamplerState NoiseSampler : register(s0);
float4 light(float3 lightPos, float3 cameraPos,float3 patchPos, float3 normal)
{
    float3 patchToLight = patchPos - lightPos;
    float3 patchToCamera = cameraPos - patchPos;
    float3 nPatchToLight = normalize(patchToLight);
    float3 nPatchToCamera = normalize(patchToCamera);
    
    float diffuse_coef = saturate(dot(normal, nPatchToLight));
    float specular_coef = pow(saturate(dot(reflect(nPatchToLight, normal), nPatchToCamera)), 4);
    return float4(diffuse_coef, specular_coef, 0, 0);

}

float4 main(VertexToPixel input) : SV_TARGET
{
    float3 lightPos = float3(0, 1000, 0);
    float3 normal = normalize(input.Normal);
    float3 patchToLight = input.WorldPosition.xyz-lightPos;
    float3 patchToCamera = CameraState.Position - input.WorldPosition.xyz;
    float3 nPatchToLight = normalize(patchToLight);
    float3 nPatchToCamera = normalize(patchToCamera);
    
    float4 l1 = light(float3(0, 100, 0), CameraState.Position, input.WorldPosition.xyz, normal);
    float4 l2 = light(float3(100, 5, 0), CameraState.Position, input.WorldPosition.xyz, normal);
    float4 l3 = light(float3(-100, 5, 0), CameraState.Position, input.WorldPosition.xyz, normal);
    float3 white = float3(1, 1, 1);
    
    float3 lcb1 = float3(1, 0, 0);
    float3 lcb2 = float3(0, 1, 0);
    float3 lcb3 = float3(0, 0, 1);

    float3 cb = Noise1.Sample(NoiseSampler, input.Texture).rgb;
    
    float3 l1c = lerp(cb, lcb1, l1.y);
    float3 l2c = lerp(cb, lcb2, l2.y);
    float3 l3c = lerp(cb, lcb3, l3.y);
    float4 lc1 = float4((l1.x + l1.y * 20) *l1c + l1.y * lcb1, 1);
    float4 lc2 = float4((l2.x + l2.y * 20) *l2c + l2.y * lcb2, 1);
    float4 lc3 = float4((l3.x + l3.y * 20) * l3c + l3.y * lcb3, 1);
    

    return float4(normalize(float4(cb * 0.3, 1) + lc1 + lc2 + lc3).xyz,1);

}