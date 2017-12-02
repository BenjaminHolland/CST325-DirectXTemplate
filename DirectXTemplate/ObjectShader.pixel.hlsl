#include <Shared.hlsli>
Texture2D Noise1 : register(t0);
SamplerState DefaultSampler : register(s0);
float4 l(float3 lightPos, float3 cameraPos, float3 patchPos, float3 normal)
{
    //float3 patchToLight = lightPos - patchPos;
    //float3 patchToCamera = cameraPos - patchPos;
    float3 patchToLight = lightPos-patchPos;
    float3 patchToCamera = cameraPos-patchPos ;
    float3 nPatchToLight = normalize(patchToLight);
    float3 nPatchToCamera = normalize(patchToCamera);
 
    float3 hv= normalize(patchToCamera - patchToLight);
    float lightOnNormal = dot(-nPatchToLight, normal);
    float halfOnNormal = dot(hv, normal);
    float zeroL = step(0, lightOnNormal);
    float diffuse = zeroL * lightOnNormal;
    float specular = pow(max(halfOnNormal, 0) * zeroL, 3) * lightOnNormal;

    return float4(diffuse, specular, 0, 0);

}

float4 main(VertexToPixel input) : SV_TARGET
{

    float3 normal = normalize(input.Normal);
   
    float time = TimeState.Discrete.x * 0.05;

    float2 l1p1 = float2(10 * cos(time), 10 * sin(time));
    float2 l2p1 = float2(25 * cos(time + 1), 10 * sin(time + 1));
    float2 l3p1 = float2(10 * cos(time + 3.141), 10 * sin(time + 3.141));

    float4 l1 = l(float3(l1p1.x, 0, l1p1.y), CameraState.Position, input.WorldPosition.xyz, normal);
    float4 l2 = l(float3(l2p1.x, 0, l2p1.y), CameraState.Position, input.WorldPosition.xyz, normal);
    float4 l3 = l(float3(l3p1.x,10, l3p1.y), CameraState.Position, input.WorldPosition.xyz, normal);
   
    
    float3 lcb1 = float3(1, 0, 0);
    float3 lcb2 = float3(0, 1, 0);
    float3 lcb3 = float3(0, 0, 1);
  
    float3 cb = Noise1.Sample(DefaultSampler, input.Texture).rgb;
    
    float3 ambient = float3(0.3, 0.3, 0.3);
    float3 ldc[4] = { ambient * cb,l1.x*cb,l2.x*cb,l3.x*cb };
    float3 ldcf = ldc[0] + ldc[1] + ldc[2] + ldc[3];
    float3 lsc[4] = { float3(0,0,0), l1.y * lcb1, l2.y * lcb2, l3.y * lcb3 };
    float3 lscf = lsc[0] + lsc[1] + lsc[2] + lsc[3];

    
    return float4(saturate(ldcf + lscf), 1);
    
    

}