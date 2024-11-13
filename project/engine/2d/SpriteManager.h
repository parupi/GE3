#pragma once
#include "DirectXManager.h"
#include <memory>
#include <mutex>
// スプライト共通部
class SpriteManager
{
public:
	static SpriteManager* instance;
	static std::once_flag initInstanceFlag;

	SpriteManager() = default;
	~SpriteManager() = default;
	SpriteManager(SpriteManager&) = default;
	SpriteManager& operator=(SpriteManager&) = default;
public:

	// シングルトンインスタンスの取得
	static SpriteManager* GetInstance();
	// 初期化
	void Initialize(DirectXManager* directXManager);
	// 描画前処理
	void DrawSet();
	// 終了
	void Finalize();

private:
	void CreateRootSignature();
	void CreateInputElementDesc();
	void CreateBlendState();
	void CreateRasterizerState();
	void LoadShader();
	void CreateDepthStencilState();
	void CreatePipelineState();
private:
	// DirectXのポインタ
	DirectXManager* dxManager_ = nullptr;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	// BlendState
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

public:
	DirectXManager* GetDxManager() const { return dxManager_; }

};