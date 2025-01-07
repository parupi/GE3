#include "TitleScene.h"
#include <Object3dManager.h>
#include <Input.h>
#include <GameScene.h>

void TitleScene::Initialize()
{
	TextureManager::GetInstance()->LoadTexture("resource/UI/TitleUI.png");
	titleUI_ = std::make_unique<Sprite>();
	titleUI_->Initialize("resource/UI/TitleUI.png");
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// シーンの切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	titleUI_->Update();
}

void TitleScene::Draw()
{
	SpriteManager::GetInstance()->DrawSet();
	titleUI_->Draw();
}
