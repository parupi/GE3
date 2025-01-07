#include "Skydome.h"
#include <ModelManager.h>

void Skydome::Initialize()
{
	ModelManager::GetInstance()->LoadModel("resource", "models/skydome/skydome.obj");
	obj_ = new Object3d();
	obj_->Initialize();

	obj_->SetModel("models/skydome/skydome.obj");
	transform_.Initialize();
}

void Skydome::Update()
{
	transform_.scale_ = { 200, 200, 200 };
	transform_.TransferMatrix();
}

void Skydome::Draw()
{
	obj_->Draw(transform_);
}
