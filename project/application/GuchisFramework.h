#pragma once
#include "WindowManager.h"
#include "DirectXManager.h"
#include "SrvManager.h"
#include "Input.h"
#include "LeakChecker.h"
#include <SceneManager.h>

class GuchisFramework
{
public:
	virtual ~GuchisFramework() = default;

public:
	// 初期化
	virtual void Initialize();
	// 終了
	virtual void Finalize();
	// 毎フレーム更新
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	// 終了チェック
	virtual bool IsEndRequest() { return winManager->ProcessMessage(); }

public:
	// 実行
	void Run();

protected:
	D3DResourceLeakChecker leakCheck;

	WindowManager* winManager = nullptr;
	DirectXManager* dxManager = nullptr;
	SrvManager* srvManager = nullptr;
	//SceneManager* sceneManager_ = nullptr;

};

