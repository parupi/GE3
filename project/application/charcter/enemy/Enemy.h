#pragma once
#include <Object3d.h>
#include <WorldTransform.h>
class Enemy
{
public:
	// 初期化
	void Initialize(const Vector3& translation);
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 衝突を検出したら呼び出されるコールバック関数
	void OoCollision();
private:
	// 接近時更新
	void MoveApproach();
	// 離脱時更新
	void MoveLeave();
private:
	Object3d* object_;
	WorldTransform transform_;

	// 生存フラグ
	bool isAlive_ = true;

	enum class Phase {
		Approach,	// 接近する
		Leave,		// 離脱する
	};
	// フェーズ
	Phase phase_ = Phase::Approach;

public:
	bool GetIsAlive() const { return isAlive_; }
	Vector3 GetWorldPosition() const;
};

