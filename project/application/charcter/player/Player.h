#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include <Input.h>
#include <PlayerBullet.h>
#include <Sprite.h>
class Player
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// Sprite描画
	void DrawUI();
private:
	// 攻撃
	void Attack();
	// 3Dレティクルの計算
	void Update3DReticle();
	// 3Dレティクルの初期化
	//void InitReticle();
	// 3Dレティクルをもとに2Dレティクルを設定
	//void Update2DReticle();
	// ワールド座標取得
	Vector3 GetWorldPosition() const;
	// レティクルワールド座標
	Vector3 GetReticlePosition();
private:
	WorldTransform transform_;
	WorldTransform transformReticle_;
	Object3d* object_;
	Object3d* objectReticle_;

	Vector3 cameraRotate;
	//Vector3 cameraTranslate;
	// 弾リスト
	std::list<PlayerBullet*> bullets_;

	//Sprite* spriteReticle_ = nullptr;
	//Vector3 reticlePosition_{};
public:
	void SetParent(const WorldTransform* parent);
	void SetOffset(const Vector3& offset) { transform_.translation_ = offset; }
	void SetCameraRotate(const Vector3& rotate) { cameraRotate = rotate; }
	//void SetCameraTranslate(const Vector3& translate) { cameraTranslate = translate; }
};

