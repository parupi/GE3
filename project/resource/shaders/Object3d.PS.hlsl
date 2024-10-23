#include "Object3d.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
    int enabled; //!< DirectionalLightの有効フラグ
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
    int enabled; //!< PointLightの有効フラグ
};
ConstantBuffer<PointLight> gPointLight : register(b3);

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    int enabled; //!< SpotLightの有効フラグ
};
ConstantBuffer<SpotLight> gSpotLight : register(b4);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    PixelShaderOutput output;

    if (gMaterial.enableLighting != 0)
    {
        float3 finalDiffuse = float3(0.0f, 0.0f, 0.0f);
        float3 finalSpecular = float3(0.0f, 0.0f, 0.0f);
        
        float3 normal = normalize(input.normal);
        
        if (gDirectionalLight.enabled != 0)
        {
            // DirectionalLightの計算
            float NdotL = dot(normal, normalize(-gDirectionalLight.direction));
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float NdotH = dot(normal, halfVector);
            float specularPow = pow(saturate(NdotH), gMaterial.shininess);

            // 拡散反射と鏡面反射を加算
            finalDiffuse += gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            finalSpecular += gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        }

        if (gPointLight.enabled != 0)
        {
            // PointLightの計算
            float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
            float distance = length(gPointLight.position - input.worldPosition);
            float factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);
            float NdotL = dot(normal, -pointLightDirection);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            float3 halfVector = normalize(-pointLightDirection + toEye);
            float NdotH = dot(normal, halfVector);
            float specularPow = pow(saturate(NdotH), gMaterial.shininess);

            // 拡散反射と鏡面反射を加算
            finalDiffuse += gPointLight.color.rgb * gPointLight.intensity * factor * gMaterial.color.rgb * textureColor.rgb * cos;
            finalSpecular += gPointLight.color.rgb * gPointLight.intensity * factor * specularPow;
        }

        if (gSpotLight.enabled != 0)
        {
            // SpotLightの計算
            float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
            float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
            float cosFalloffStart = gSpotLight.cosAngle * 2.0f;
            float falloffFactor = pow(saturate((cosAngle - gSpotLight.cosAngle) / (cosFalloffStart - gSpotLight.cosAngle)), gSpotLight.decay);
            float NdotL = dot(normal, -spotLightDirectionOnSurface);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            float3 halfVector = normalize(-spotLightDirectionOnSurface + toEye);
            float NdotH = dot(normal, halfVector);
            float specularPow = pow(saturate(NdotH), gMaterial.shininess);

            // 拡散反射と鏡面反射を加算
            finalDiffuse += gSpotLight.color.rgb * gSpotLight.intensity * falloffFactor * gMaterial.color.rgb * textureColor.rgb * cos;
            finalSpecular += gSpotLight.color.rgb * gSpotLight.intensity * falloffFactor * specularPow;
        }

        // 拡散反射 + 鏡面反射
        output.color.rgb = finalDiffuse + finalSpecular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    return output;
}