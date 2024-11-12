#include "Enemy.h"

void Enemy::Initialize(const Vector3& translation)
{
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("cube.obj");
	object_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	transform_.Initialize();
	transform_.translation_ = translation;
	transform_.scale_ *= 0.5f;
	transform_.TransferMatrix();
}

void Enemy::Finalize()
{
	delete object_;
}

void Enemy::Update()
{
	switch (phase_) {
	case Phase::Approach:
	default:
		MoveApproach();
		break;
	case Phase::Leave:
		MoveLeave();
		break;
	}

	// 行列の更新
	transform_.TransferMatrix();
}

void Enemy::Draw()
{
	object_->Draw(transform_);
}

void Enemy::OoCollision()
{
	// 衝突時にデスフラグを立てる
	isAlive_ = false;
}

void Enemy::MoveApproach()
{
	if (object_->GetColor().w < 1.0f) {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, object_->GetColor().w + 0.1f });
	}
	else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	}
}

void Enemy::MoveLeave()
{
	if (object_->GetColor().w > 0.0f) {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, object_->GetColor().w - 0.03f });
	}
	else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	}
}

Vector3 Enemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = transform_.matWorld_.m[3][0];
	worldPos.y = transform_.matWorld_.m[3][1];
	worldPos.z = transform_.matWorld_.m[3][2];

	return worldPos;
}
