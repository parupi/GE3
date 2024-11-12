#include "PlayerBullet.h"

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity)
{
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel("plane.obj");

	transform_.Initialize();
	transform_.translation_ = position;
	transform_.TransferMatrix();
	velocity_ = velocity;
}

void PlayerBullet::Finalize()
{
}

void PlayerBullet::Update()
{
	// 座標を移動させる（1フレーム分の移動量を足し込む）
	transform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 行列を更新
	transform_.TransferMatrix();
}

void PlayerBullet::Draw()
{
	object_->Draw(transform_);
}
