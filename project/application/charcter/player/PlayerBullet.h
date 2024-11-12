#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include <memory>
class PlayerBullet
{
public:
	// 初期化
	void Initialize(const Vector3& position, const Vector3& velocity);
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	WorldTransform transform_;
	std::unique_ptr<Object3d> object_;

	// 速度
	Vector3 velocity_;
	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

public:
	// 消滅フラグを取得
	bool GetIsDead() const { return isDead_; }
};

