#pragma once
#include <Windows.h>
#include <string>
class WindowManager
{
public:
	// クライアント領域のサイズ
	static const uint32_t kClientWidth = 1280;
	static const uint32_t kClientHeight = 720;

public: // 静的メンバ変数

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

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
