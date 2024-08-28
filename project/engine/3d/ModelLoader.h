#pragma once
#include "DirectXManager.h"
class ModelLoader
{
public: // メンバ関数
	void Initialize(DirectXManager* dxManager);

private:

	DirectXManager* dxManager_;

public: // ゲッター//セッター//
	DirectXManager* GetDxManager() const { return dxManager_; }

};

