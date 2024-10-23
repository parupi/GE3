#include "LightManager.h"
#include <Object3dManager.h>

void LightManager::Initialize()
{
    dxManager_ = Object3dManager::GetInstance()->GetDxManager();
}

void LightManager::AddDirectionalLight(const DirectionalLight& light)
{
	dirLights_.push_back(light);
}

void LightManager::AddPointLight(const PointLight& light)
{
	pointLights_.push_back(light);
}

void LightManager::AddSpotLight(const SpotLight& light)
{
	spotLights_.push_back(light);
}

void LightManager::UpdateLightResources()
{
    // ディレクショナルライトのリソースを作成または更新
    if (dirLights_.size() != 0 && !dirLightResource_ && dirLights_.size() < maxDirLights) {
        size_t size = dirLights_.size() * sizeof(DirectionalLight);
        dirLightResource_ = dxManager_->CreateBufferResource(size);
        dirLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&dirLightData_));
    }
    std::memcpy(dirLightData_, dirLights_.data(), dirLights_.size() * sizeof(DirectionalLight));

    // ポイントライトのリソースを作成または更新
    if (!pointLightResource_ && pointLights_.size() < maxPointLights) {
        size_t size = pointLights_.size() * sizeof(PointLight);
        pointLightResource_ = dxManager_->CreateBufferResource(size);
        pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
    }
    std::memcpy(pointLightData_, pointLights_.data(), pointLights_.size() * sizeof(PointLight));

    // スポットライトのリソースを作成または更新
    if (!spotLightResource_ && spotLights_.size() < maxSpotLights) {
        size_t size = spotLights_.size() * sizeof(SpotLight);
        spotLightResource_ = dxManager_->CreateBufferResource(size);
        spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
    }
    std::memcpy(spotLightData_, spotLights_.data(), spotLights_.size() * sizeof(SpotLight));
}

void LightManager::BindLightsToShader()
{
    auto commandList = dxManager_->GetCommandList();

    // ポイントライトのリソースをシェーダーにバインド
    commandList->SetGraphicsRootConstantBufferView(3, dirLightResource_->GetGPUVirtualAddress());

    // ポイントライトのリソースをシェーダーにバインド
    commandList->SetGraphicsRootConstantBufferView(5, pointLightResource_->GetGPUVirtualAddress());

    // スポットライトのリソースをシェーダーにバインド
    commandList->SetGraphicsRootConstantBufferView(6, spotLightResource_->GetGPUVirtualAddress());
}
