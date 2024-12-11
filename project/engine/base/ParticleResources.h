#pragma once
#include <DirectXManager.h>
#include <SrvManager.h>
#include "Camera.h"
class ParticleResources
{
private:
	static ParticleResources* instance;

	ParticleResources() = default;
	~ParticleResources() = default;
	ParticleResources(ParticleResources&) = default;
	ParticleResources& operator=(ParticleResources&) = default;
public:
	// シングルトンインスタンスの取得
	static ParticleResources* GetInstance();
	// 初期化
	void Initialize(DirectXManager* dxManager, SrvManager* srvManager);
	// 終了
	void Finalize();
	// 描画前処理
	void DrawSet();
private:

	void CreateRootSignature();
	void CreateInputElementDesc();
	void CreateBlendState();
	void CreateRasterizerState();
	void LoadShader();
	void CreatePipelineState();

private:
	// DxManager
	DirectXManager* dxManager_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	Camera* camera_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	// BlendState
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

public:
	DirectXManager* GetDxManager() { return dxManager_; }
	SrvManager* GetSrvManager() { return srvManager_; }
	Camera* GetCamera() { return camera_; }
	void SetCamera(Camera* camera) { camera_ = camera; }
};

