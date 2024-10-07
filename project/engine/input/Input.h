#pragma once
#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION		0x0800	// DirectXのバージョン指定
#include <dinput.h>

#include "WindowManager.h"

/// <summary>
/// 入力処理
/// </summary>
class Input 
{
private:
	static Input* instance;

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = default;
public:
	// シングルトンインスタンスの取得
	static Input* GetInstance();

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 初期化
	void Initialize(WindowManager* winManager);
	// 終了
	void Finalize();
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
	// WindowsAPI
	WindowManager* winManager_ = nullptr;
};