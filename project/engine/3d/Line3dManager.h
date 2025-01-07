#pragma once
#include "DirectXManager.h"
#include <Camera.h>
#include <memory>
#include <mutex>
class Line3dManager
{
private:
	static Line3dManager* instance;
	static std::once_flag initInstanceFlag;

	Line3dManager() = default;
	~Line3dManager() = default;
	Line3dManager(Line3dManager&) = default;
	Line3dManager& operator=(Line3dManager&) = default;
public:
	// シングルトンインスタンスの取得
	static Line3dManager* GetInstance();
	// 初期化
	void Initialize(DirectXManager* directXManager);
	// 終了
	void Finalize();
	// 描画前処理
	void DrawSet();

public: // アクセッサ
	DirectXManager* GetDxManager() { return dxManager_; }

private:
	void CreateRootSignature();
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

};