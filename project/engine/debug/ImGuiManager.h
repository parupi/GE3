#pragma once
#include "WindowManager.h"
#include "DirectXManager.h"
#include <memory>
class ImGuiManager
{
private:
	static std::unique_ptr<ImGuiManager> instance;
public:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(ImGuiManager&) = delete;
	ImGuiManager& operator=(ImGuiManager&) = delete;
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

