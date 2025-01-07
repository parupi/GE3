#include "Line3d.hlsli"

cbuffer Transform : register(b0)
{
    matrix WorldViewProjection;
};

struct VertexShaderInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(float4(input.position, 1.0), WorldViewProjection);
    output.color = input.color;
    return output;
}
