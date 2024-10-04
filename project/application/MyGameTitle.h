#pragma once

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#include "Input.h"
#include "WindowManager.h"
#include "DirectXManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Object3d.h"
#include "Object3dmanager.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelManager.h"
#include "SrvManager.h"
#include <ParticleManager.h>
#include "ImGuiManager.h"
#include "LeakChecker.h"
class MyGameTitle
{
public: 
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了リクエストを送る
	bool IsEndRequest();
private:

	D3DResourceLeakChecker leakCheck;

	// ポインタ 
	WindowManager* winManager = nullptr;
	DirectXManager* directXManager = nullptr;
	SrvManager* srvManager = nullptr;
	SpriteManager* spriteManager = nullptr;
	Object3dManager* objectManager = nullptr;
	Input* input = nullptr;
	std::vector<Object3d*> objects;
	std::vector<Model*> models;
	std::vector<Sprite*> sprites;
	Camera* camera;

	// エンドフラグ
	bool isEndRequest = false;
};

