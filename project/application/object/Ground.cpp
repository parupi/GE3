#include "Ground.h"
#include "ModelManager.h"
#include <imgui.h>

void Ground::Initialize()
{
	ModelManager::GetInstance()->LoadModel("resource", "Ground.obj");
	obj_ = new Object3d();
	obj_->Initialize();
	obj_->SetModel("Ground.obj");
	obj_->SetIsLighting(false);

	transform_.Initialize();
}

void Ground::Update()
{
	transform_.TransferMatrix();
}

void Ground::Draw()
{
	obj_->Draw(transform_);
}
