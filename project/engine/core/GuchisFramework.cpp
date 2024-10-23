#include "GuchisFramework.h"

void GuchisFramework::Initialize()
{
	// WinDowsAPIの初期化
	winManager = std::make_unique<WindowManager>();
	winManager->Initialize();
	// DirectXの初期化
	dxManager = std::make_unique<DirectXManager>();
	dxManager->Initialize(winManager.get());
	// SRVマネージャーの初期化
	srvManager = std::make_unique<SrvManager>();
	srvManager->Initialize(dxManager.get());
	// 入力の初期化
	Input::GetInstance()->Initialize(winManager.get());
	// Audioの初期化
	Audio::GetInstance()->Initialize();
}

void GuchisFramework::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	Input::GetInstance()->Finalize();
	winManager->Finalize();
	dxManager->Finalize();
}

void GuchisFramework::Update()
{
	SceneManager::GetInstance()->Update();
	Input::GetInstance()->Update();
}

void GuchisFramework::Run()
{
	Initialize();

	while (true) {
		Update();

		//Windowにメッセージが来てたら最優先で処理させる
		if (IsEndRequest()) {
			// ゲームループを抜ける
			break;
		}

		Draw();
	}
	Finalize();
}
