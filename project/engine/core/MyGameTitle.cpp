#include "MyGameTitle.h"
#include <SceneFactory.h>

void MyGameTitle::Initialize()
{
	GuchisFramework::Initialize();
	// ImGui初期化
	ImGuiManager::GetInstance()->Initialize(winManager.get(), dxManager.get());
	// 2Dテクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxManager.get(), srvManager.get());
	// 3Dテクスチャマネージャーの初期化
	ModelManager::GetInstance()->Initialize(dxManager.get());
	// パーティクルマネージャーの初期化
	ParticleManager::GetInstance()->Initialize(dxManager.get(), srvManager.get());
	// スプライト共通部の初期化
	SpriteManager::GetInstance()->Initialize(dxManager.get());
	// オブジェクト共通部
	Object3dManager::GetInstance()->Initialize(dxManager.get());

	// 最初のシーンを生成
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	// シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

	// インスタンス生成
	GlobalVariables::GetInstance();
}

void MyGameTitle::Finalize()
{
	//ParticleManager::GetInstance()->Finalize();
	SpriteManager::GetInstance()->Finalize();

	ImGuiManager::GetInstance()->Finalize();
	GuchisFramework::Finalize();
}

void MyGameTitle::Update()
{
	ImGuiManager::GetInstance()->Begin();
	GuchisFramework::Update();


	ImGuiManager::GetInstance()->End();
}

void MyGameTitle::Draw()
{
	dxManager->BeginDraw();
	srvManager->BeginDraw();

	SceneManager::GetInstance()->Draw();

	ImGuiManager::GetInstance()->Draw();

	dxManager->EndDraw();
}

