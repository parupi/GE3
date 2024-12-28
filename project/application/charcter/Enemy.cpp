#include "Enemy.h"

void Enemy::Initialize() 
{
	object_ = std::make_unique<Object3d>();
	object_->Initialize("plane.obj");

	transform_.Initialize();
}

void Enemy::Finalize() 
{

}

void Enemy::Update() 
{
	transform_.TransferMatrix();
}

void Enemy::Draw() 
{
	object_->Draw(transform_);
}