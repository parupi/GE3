#include "Rail.h"

void Rail::Initialize()
{
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("plane.obj");

	transform_.Initialize();
}

void Rail::Finalize()
{
	delete object_;
}

void Rail::Update()
{
	transform_.TransferMatrix();
}

void Rail::Draw()
{
	object_->Draw(transform_);
}
