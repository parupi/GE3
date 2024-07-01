#pragma once
#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION		0x0800	// DirectXのバージョン指定
#include <dinput.h>

/// <summary>
/// 入力処理
/// </summary>
class Input 
{
public: // メンバ関数
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 初期化
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	// 更新
	void Update();

private:
	ComPtr<IDirectInputDevice8> keyboard;
};