#pragma once
#include <vector>
#include <Vector4.h>
#include <Vector3.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXManager.h>

constexpr int maxDirLights = 100;
constexpr int maxPointLights = 100; // ポイントライトの最大数
constexpr int maxSpotLights = 50;   // スポットライトの最大数

struct DirectionalLight {
	Vector4 color;		//!< ライトの色
	Vector3 direction;	//!< ライトの向き
	float intensity;	//!< 輝度
	int enabled;
};

// pointLight
struct PointLight {
	Vector4 color; //!<ライトの色
	Vector3 position; //!<ライトの位置
	float intensity; //!< 輝度
	float radius; //!< ライトの届く最大距離
	float decay; //!< 減衰率
	int enabled;
	float padding[2];
};

// spotLight
struct SpotLight {
	Vector4 color; //!< ライトの色
	Vector3 position; //!< ライトの位置
	float intensity; //!< 輝度
	Vector3 direction; //!< ライトの向き
	float distance; //!< ライトの届く最大距離
	float decay; //!< 減衰率
	float cosAngle; //!< スポットライトの余弦
	int enabled;
	float padding[2];
};

class LightManager
{
public:
	void Initialize();
	void AddDirectionalLight(const DirectionalLight& light);
	void AddPointLight(const PointLight& light);
	void AddSpotLight(const SpotLight& light);
	void UpdateLightResources();
	void BindLightsToShader();

private:
	DirectXManager* dxManager_ = nullptr;

	std::vector<DirectionalLight> dirLights_;
	std::vector<PointLight> pointLights_;
	std::vector<SpotLight> spotLights_;

	Microsoft::WRL::ComPtr<ID3D12Resource> dirLightResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_ = nullptr;

	DirectionalLight* dirLightData_ = nullptr;
	PointLight* pointLightData_ = nullptr;
	SpotLight* spotLightData_ = nullptr;


};

