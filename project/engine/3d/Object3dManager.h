#pragma once
#include "DirectXManager.h"
#include <Camera.h>
class Object3dManager
{
	static Object3dManager* instance;

	Object3dManager() = default;
	~Object3dManager() = default;
	Object3dManager(Object3dManager&) = default;
	Object3dManager& operator=(Object3dManager&) = default;
public:
	// シングルトンインスタンスの取得
	static Object3dManager* GetInstance();
	// 初期化
	void Initialize(DirectXManager* directXManager);
	// 描画前処理
	void DrawSet();
	// 終了
	void Finalize();
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
	// カメラのポインタ
	Camera* defaultCamera_ = nullptr;

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

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

public: // ゲッター // セッター //
	DirectXManager* GetDxManager() const { return dxManager_; }
	// デフォルトカメラ
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }
	Camera* GetDefaultCamera() const { return defaultCamera_; }
};

