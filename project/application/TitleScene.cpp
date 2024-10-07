#include "TitleScene.h"
#include <Object3dManager.h>

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
}

void TitleScene::Draw()
{
}
