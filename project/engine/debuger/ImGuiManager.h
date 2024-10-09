#pragma once
#include "WindowManager.h"
#include "DirectXManager.h"
#include <mutex>
class ImGuiManager
{
private:
	static ImGuiManager* instance;
	static std::once_flag initInstanceFlag;

	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(ImGuiManager&) = default;
	ImGuiManager& operator=(ImGuiManager&) = default;
public:
	// シングルトンインスタンスの取得
	static ImGuiManager* GetInstance();
	// 初期化
	void Initialize(WindowManager* winManager, DirectXManager* directXManager);
	// ImGui受付開始
	void Begin();
	// ImGui受付終了
	void End();
	// 描画
	void Draw();
	// 終了
	void Finalize();
private:
	WindowManager* winManager_ = nullptr;
	DirectXManager* dxManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

