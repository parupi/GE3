#include "Line3d.hlsli"

struct PSInput
{
    float4 position : SV_POSITION; // 必須: 頂点シェーダーからのポジション
    float4 color : COLOR; // 必須: 頂点シェーダーからのカラー
};

float4 main(PSInput input) : SV_TARGET
{
    return input.color;
}
