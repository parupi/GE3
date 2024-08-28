#include <Windows.h>
#include <dxgidebug.h>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include <cstdint>

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

struct D3DResourceLeakChecker {
	~D3DResourceLeakChecker() {
		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;

	// ポインタ 
	WindowManager* winManager = nullptr;
	DirectXManager* directXManager = nullptr;
	SpriteManager* spriteManager = nullptr;
	Object3dManager* objectManager = nullptr;
	Input* input = nullptr;

	// WinDowsAPIの初期化
	winManager = new WindowManager();
	winManager->Initialize();
	// DirectXの初期化
	directXManager = new DirectXManager();
	directXManager->Initialize(winManager);

	// 2Dテクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(directXManager);
	// 3Dテクスチャマネージャーの初期化
	ModelManager::GetInstance()->Initialize(directXManager);

	// スプライト共通部の初期化
	spriteManager = new SpriteManager();
	spriteManager->Initialize(directXManager);
	// オブジェクト共通部
	objectManager = new Object3dManager();
	objectManager->Initialize(directXManager);

	// Textureのロード
	TextureManager::GetInstance()->LoadTexture("resource/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resource/monsterBall.png");
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	// Spriteの初期化
	std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		if (i == 1) {
			sprite->Initialize(spriteManager, "resource/monsterBall.png");
		}
		else {
			sprite->Initialize(spriteManager, "resource/uvChecker.png");
		}

		Vector2 initialPosition = Vector2{ i * 50.0f, 0.0f }; // 各スプライトを100ピクセルずつ右にずらして配置
		sprite->SetPosition(initialPosition);

		sprites.push_back(sprite);
	}

	std::vector<Object3d*> objects;
	std::vector<Model*> models;

	for (int i = 0; i < 2; ++i) {
		// Modelの初期化
		Model* model = new Model();
		if (i == 1) {
			model->Initialize(ModelManager::GetInstance()->GetModelLoader(), "resource", "axis.obj");
		}
		else {
			model->Initialize(ModelManager::GetInstance()->GetModelLoader(), "resource", "plane.obj");
		}
		models.push_back(model);

		// Object3dの初期化
		Object3d* object = new Object3d();
		object->Initialize(objectManager);
		object->SetModel(model);
		objects.push_back(object);
	}

	// それぞれのObject3dに位置や回転を設定
	models[0]->SetPosition({ -2.0f, 0.0f, 0.0f }); 
	models[0]->SetRotation({ 0.0f, 0.0f, 0.0f }); 

	models[1]->SetPosition({ 2.0f, 0.0f, 0.0f }); 
	models[1]->SetRotation({ 0.0f, 45.0f, 0.0f }); 

	// 入力の初期化
	input = new Input();
	input->Initialize(winManager);

	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (winManager->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		// ゲームの処理

		for (Sprite* sprite : sprites) {
			// 平行移動用処理
			//Vector2 position = sprite->GetPosition();
			//position += Vector2{ 1.0f, 1.0f };
			//sprite->SetPosition(position);
			// 回転
			//float rotation = sprite->GetRotation();
			//rotation += 0.01f;
			//sprite->SetRotation(rotation);
			// 拡縮
			Vector2 size = sprite->GetSize();
			size.x = 50.0f;
			size.y = 50.0f;
			sprite->SetSize(size);
			// 色
			//Vector4 color = sprite->GetColor();
			//color.x += 0.01f;
			//if (color.x > 1.0f) {
			//	color.x -= 1.0f;
			//}
			//sprite->SetColor(color);
			//sprite->SetIsFlipX(true);
			//sprite->SetIsFlipY(true);
			sprite->Update();
		}


		for (auto& object : objects) {
			object->Update();
		}



		directXManager->BeginDraw();

		objectManager->DrawSet();

		for (auto& object : objects) {
			object->Draw();
		}


		spriteManager->DrawSet();

		for (Sprite* sprite : sprites) {
			sprite->Draw();
		}

		// 実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXManager->GetCommandList().Get());

		directXManager->EndDraw();
	}

	// ImGuiの終了処理。
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete input;
	input = nullptr;
	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	delete spriteManager;
	spriteManager = nullptr;
	delete objectManager;
	objectManager = nullptr;

	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	for (Object3d* object : objects) {
		delete object;
		object = nullptr;
	}
	for (Model* model : models) {
		delete model;
		model = nullptr;
	}
	delete directXManager;
	directXManager = nullptr;
	winManager->Finalize();
	delete winManager;
	winManager = nullptr;

	return 0;
}
