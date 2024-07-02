#pragma once
#include <Windows.h>
class WindowManager
{
public:
	// クライアント領域のサイズ
	static const int kClientWidth = 1280;
	static const int kClientHeight = 720;

public: // メンバ変数
	// 初期化
	void Initialize();
	// 更新
	void Update();

private:
	// Window関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASSEX wndClass_{}; // ウィンドウクラス
};

