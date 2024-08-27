#pragma once
#include "DirectXManager.h"

// スプライト共通部
class SpriteManager
{
public:
	void Initialize(DirectXManager* directXManager);


	void DrawSet();

private:
	void CreateRootSignature();
	D3D12_INPUT_LAYOUT_DESC CreateInputElementDesc();
	void CreateBlendState();
	void CreateRasterizerState();
	void LoadShader();
	void CreatePipelineState();
private:
	// DirectXのポインタ
	DirectXManager* dxManager_ = nullptr;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	// BlendState
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

public:
	DirectXManager* GetDxManager() const { return dxManager_; }

};