#include "MyGameTitle.h"

void MyGameTitle::Initialize()
{
	GuchisFramework::Initialize();
	// ImGui初期化
	ImGuiManager::GetInstance()->Initialize(winManager, dxManager);
	// 2Dテクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxManager, srvManager);
	// 3Dテクスチャマネージャーの初期化
	ModelManager::GetInstance()->Initialize(dxManager);
	// パーティクルマネージャーの初期化
	//ParticleManager::GetInstance()->Initialize(directXManager, srvManager);
	// スプライト共通部の初期化
	SpriteManager::GetInstance()->Initialize(dxManager);
	// オブジェクト共通部
	Object3dManager::GetInstance()->Initialize(dxManager);

	titleScene_ = new TitleScene();
	titleScene_->Initialize();

	//gameScene_ = new GameScene();
	//gameScene_->Initialize();
	
}

void MyGameTitle::Finalize()
{
	//gameScene_->Finalize();
	titleScene_->Finalize();

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	//ParticleManager::GetInstance()->Finalize();
	SpriteManager::GetInstance()->Finalize();
	Object3dManager::GetInstance()->Finalize();

	ImGuiManager::GetInstance()->Finalize();
	GuchisFramework::Finalize();
}

void MyGameTitle::Update()
{
	GuchisFramework::Update();
	ImGuiManager::GetInstance()->Begin();

	titleScene_->Update();
	//gameScene_->Update();

	ImGuiManager::GetInstance()->End();
}

void MyGameTitle::Draw()
{
	dxManager->BeginDraw();
	srvManager->BeginDraw();

	titleScene_->Draw();
	//gameScene_->Draw();

	ImGuiManager::GetInstance()->Draw();

	dxManager->EndDraw();
}

