#include "Player.h"

void Player::Initialize()
{
	object_ = new Object3d();
	object_->Initialize();
	object_->SetModel("plane.obj");

	objectReticle_ = new Object3d();
	objectReticle_->Initialize();
	objectReticle_->SetModel("plane.obj");

	//TextureManager::GetInstance()->LoadTexture("resource/rail.png");
	//spriteReticle_ = new Sprite();
	//spriteReticle_->Initialize("resource/rail.png");
	//spriteReticle_->SetSize({ 32.0f, 32.0f });
	//spriteReticle_->SetPosition({ 32.0f, 32.0f });

	transform_.Initialize();
	transformReticle_.Initialize();

}

void Player::Finalize()
{
	delete object_;
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


	Update3DReticle();
	//Update2DReticle();
	Attack();
	//spriteReticle_->Update();
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

void Player::DrawUI()
{
	//spriteReticle_->Draw();
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
	const float kReticleMoveSpeed = 0.3f;
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

//void Player::InitReticle()
//{
//
//
//	//// 自機から3Dレティクルへのオフセット
//	//Vector3 offset = { 0.0f, 0.0f, 1.0f };
//	//// 自機のワールド行列の回転を反映
//	//cameraRotate.y -= 1.57f;
//	//offset = TransformNormal(offset, MakeRotateXYZMatrix(cameraRotate));
//	//// 正規化
//	//offset = Normalize(offset) * kDistancePlayerToReticle;
//
//	//// レティクル位置の基準となる初期位置を設定
//	//reticlePosition_ = offset;
//	//transformReticle_.translation_ = reticlePosition_;
//	//transformReticle_.translation_.x = transform_.translation_.x + kDistancePlayerToReticle;
//	//transformReticle_.TransferMatrix();
//}

//void Player::Update2DReticle()
//{
//	Vector3 positionReticle = GetReticlePosition();
//	// ビューポート行列
//	Matrix4x4 matViewport = MakeViewPortMatrix(0, 0, 1280, 720, 0, 1);
//	// 各行列を合成
//	Matrix4x4 matView = CreateViewMatrix(cameraTranslate, cameraRotate);
//	Matrix4x4 matProjection = CreateProjectionMatrix(0.45f, float(WindowManager::kClientWidth) / float(WindowManager::kClientHeight), 0.1f, 200.0f);
//	Matrix4x4 matViewProjectionViewport = matViewport * matProjection * matView; // ビューポート行列を最初に掛け合わせ
//	// ワールドからスクリーンに変換
//	positionReticle = TransformFunc(positionReticle, matViewProjectionViewport);
//	// 座標設定
//	spriteReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
//}

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
	//transformReticle_.parent_ = parent;
}
