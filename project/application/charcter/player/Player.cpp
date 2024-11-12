#include "Player.h"

void Player::Initialize()
{
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("plane.obj");

	objectReticle_ = new Object3d();
	objectReticle_->Initialize();
	objectReticle_->SetModel("planeReticle.obj");

	transform_.Initialize();
 
	transformReticle_.Initialize();
}

void Player::Finalize()
{
	delete object_;
	delete objectReticle_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Update()
{
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	transform_.translation_.x = -3.0f;
	Update3DReticle();
	Attack();
	transform_.TransferMatrix();
}

void Player::Draw()
{
	object_->Draw(transform_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
	objectReticle_->Draw(transformReticle_);
}

void Player::Attack()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, transform_.matWorld_);
		// 自機から昇順オブジェクトへのベクトル
		velocity = GetReticlePosition() - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Update3DReticle()
{
	// 自機からレティクルへの距離
	const float kDistancePlayerToReticle = 50.0f;
	// レティクルの移動速度
	const float kReticleMoveSpeed = 0.5f;
	// キー入力によるレティクル移動処理
	if (Input::GetInstance()->PushKey(DIK_W)) {
		transformReticle_.translation_.y += kReticleMoveSpeed;  // Wキーで奥へ移動
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		transformReticle_.translation_.y -= kReticleMoveSpeed;  // Sキーで手前へ移動
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		transformReticle_.translation_.z += kReticleMoveSpeed;  // Aキーで左へ移動
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		transformReticle_.translation_.z -= kReticleMoveSpeed;  // Dキーで右へ移動
	}

	transformReticle_.translation_.x = GetWorldPosition().x + kDistancePlayerToReticle;
	transformReticle_.rotation_.y = 1.57f;
	// 3Dレティクルの座標を設定
	transformReticle_.TransferMatrix();
}

Vector3 Player::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動行列を（ワールド座標）
	worldPos.x = transform_.matWorld_.m[3][0];
	worldPos.y = transform_.matWorld_.m[3][1];
	worldPos.z = transform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetReticlePosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動行列を（ワールド座標）
	worldPos.x = transformReticle_.matWorld_.m[3][0];
	worldPos.y = transformReticle_.matWorld_.m[3][1];
	worldPos.z = transformReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent)
{
	// 親子関係を結ぶ
	transform_.parent_ = parent;
}
