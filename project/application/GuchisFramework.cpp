#include "GuchisFramework.h"

void GuchisFramework::Initialize()
{
	// WinDowsAPIの初期化
	winManager = new WindowManager();
	winManager->Initialize();
	// DirectXの初期化
	dxManager = new DirectXManager();
	dxManager->Initialize(winManager);
	// SRVマネージャーの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxManager);
	// 入力の初期化
	input = new Input();
	input->Initialize(winManager);
}

void GuchisFramework::Finalize()
{
	delete input;
	input = nullptr;

	delete srvManager;
	srvManager = nullptr;
	delete dxManager;
	dxManager = nullptr;
	winManager->Finalize();
	delete winManager;
	winManager = nullptr;
}

void GuchisFramework::Update()
{






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