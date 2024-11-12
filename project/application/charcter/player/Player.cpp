#include "Player.h"

void Player::Initialize()
{
	transform_.Initialize();
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("plane.obj");
}

void Player::Update()
{

	transform_.TransferMatrix();
}

void Player::Draw()
{
	object_->Draw(transform_);

}

void Player::SetParent(const WorldTransform* parent)
{
	// 親子関係を結ぶ
	transform_.parent_ = parent;
}
