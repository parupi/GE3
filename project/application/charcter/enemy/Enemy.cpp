#include "Enemy.h"
#include "charcter/player/Weapon.h"
#include "charcter/player/Player.h"

void Enemy::Initialize(const Vector3& pos)
{
	object_ = std::make_unique<Object3d>();
	object_->Initialize("float_body.obj");

	transform_.Initialize();
	transform_.translation_ = pos;
	transform_.TransferMatrix();

	shadeObject_ = std::make_unique<Object3d>();
	shadeObject_->Initialize("float_body.obj");
	shadeObject_->SetColor({ 0.0f, 0.0f, 0.0f, 0.9f });

	shadeTransform_.Initialize();
	shadeTransform_.scale_ = { 1.0f, 0.01f, 1.0f };

	Collider::Initialize();
	// 種別のIDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
}

void Enemy::Finalize() 
{

}

void Enemy::Update() 
{
	object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	preIsAlive = isAlive;

	vel_ += gravity * deltaTime;

	const float friction = 0.9f;

	// 摩擦による速度減衰 (X方向とZ方向に適用)
	vel_.x *= pow(1.0f - friction, deltaTime);
	vel_.z *= pow(1.0f - friction, deltaTime);

	// 極端に小さい値をゼロにする（停止状態を安定化）
	if (fabs(vel_.x) < 0.1f) vel_.x = 0.0f;
	if (fabs(vel_.z) < 0.1f) vel_.z = 0.0f;

	// 位置を更新
	transform_.translation_ += vel_ * deltaTime;

	// 地面との衝突を考慮
	if (transform_.translation_.y < 0.0f) {
		transform_.translation_.y = 0.0f; // 地面上に固定
		vel_.y = -vel_.y * 0.2f; // 反発係数を適用
		if (fabs(vel_.y) < 0.01f) vel_.y = 0.0f;
	}

	BehaviorInitialize();

	BehaviorUpdate();

	shadeTransform_.translation_ = transform_.translation_;
	shadeTransform_.translation_.y = 0.0f;
	shadeTransform_.TransferMatrix();

	if (isAlive) {
		DeadUpdate();
	}
}

void Enemy::Draw() 
{
	if (!isAlive) {
		return;
	}

	object_->Draw(transform_);
	shadeObject_->Draw(shadeTransform_);
}

void Enemy::Move()
{
	speed_ = kSpeed_;
	
	Vector3 playerPos = player_->GetTranslate();
	//playerPos = playerPos - Vector3{ 0,1.0f,0 };
	Vector3 pos = { playerPos - transform_.translation_ };
	float weponRadius = 4.0f;
	if (Length(pos) > weponRadius + 1.0f)// プレイヤーの半径 + エネミー半径 - 0.1fくらい？
	{
		pos = Normalize(pos);
		transform_.translation_ += pos * speed_;
	}
	Turning();
}

void Enemy::Turning()
{
	Vector3 lockOnPos = player_->GetTranslate();
	Vector3 sub = lockOnPos - transform_.translation_;
	transform_.rotation_.y = std::atan2(sub.x, sub.z);
}

void Enemy::BehaviorInitialize()
{
	if (behaviortRquest_) {
		// 振る舞いを変更する
		behavior_ = behaviortRquest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case EnemyBehavior::kRoot:
		default:
			BehaviorRootInit();
			break;
		case EnemyBehavior::kKnockBack:
			BehaviorKnockBackInit();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviortRquest_ = std::nullopt;
	}
}

void Enemy::BehaviorUpdate()
{
	switch (behavior_) {
	case EnemyBehavior::kRoot:
	default:
		// 通常行動更新
		BehaviorRootUpdate();
		break;
	case EnemyBehavior::kKnockBack:
		BehaviorKnockBackUpdate();
		break;
	}
}

void Enemy::BehaviorRootInit()
{
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	onCollision_ = false;
	Turning();
}

void Enemy::BehaviorKnockBackInit()
{
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kNone));
	// プレイヤーの正面方向を取得（例: ワールド空間での前方向）
	Vector3 frontDir = Normalize(player_->GetFrontDirection()); // プレイヤーの正面方向のベクトルを取得する関数

	// 正面方向にノックバックの大きさを掛ける
	Vector3 knockBackForce = frontDir * player_->GetKnockBack().x;
	vel_.y += player_->GetKnockBack().y;

	// ノックバックを適用
	vel_.x += knockBackForce.x;
	vel_.z += knockBackForce.z;
}

void Enemy::BehaviorRootUpdate()
{
	if (onCollision_)
	{
		behaviortRquest_ = EnemyBehavior::kKnockBack;
	}
	Move();
	transform_.TransferMatrix();
}

void Enemy::BehaviorKnockBackUpdate()
{
	// 速度を更新
	transform_.TransferMatrix();

	if (player_->GetIsChangeState())
	{
		behaviortRquest_ = EnemyBehavior::kRoot;
	}
}

void Enemy::DeadUpdate()
{
	if (hp_ <= 0.0f) {
		isAlive = false;
	}

}

Vector3 Enemy::GetCenterPosition() const
{
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, transform_.matWorld_);
	return worldPos;
}

void Enemy::OnCollision(Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon)) {

		if (player_->GetAttackFlag()) {
			onCollision_ = true;
			hp_ -= player_->GetDamage();
			object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
	}
}

bool Enemy::IsDeadTriger()
{
	if (!isAlive) {
		if (preIsAlive) {
			return true;
		}
	}
	return false;
}
