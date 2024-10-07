#include "TitleScene.h"
#include <Object3dManager.h>
#include <Input.h>
#include <GameScene.h>

void TitleScene::Initialize()
{
	// カメラの生成
	camera = new Camera();
	Object3dManager::GetInstance()->SetDefaultCamera(camera);

}

void TitleScene::Finalize()
{
	delete camera;
	camera = nullptr;

}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// ゲームプレイシーン(次シーン)を生成
		//BaseScene* scene = new GameScene();
		// シーンの切り替え依頼
		//sceneManager_->SetNextScene(scene);
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{
}
