#include "Sphere.h"

void Sphere::Initialize()
{
	worldTransform_.Initialize();
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("sphere.obj");
}

void Sphere::Update()
{
	worldTransform_.scale_ = { 300, 300, 300 };
	worldTransform_.TransferMatrix();
}

void Sphere::Draw()
{
	object_->Draw(worldTransform_);
}
