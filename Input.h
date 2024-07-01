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

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>トリガーか否か</returns>
	bool TriggerKey(BYTE keyNumber);

private:
	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};