#pragma once
#include <WorldTransform.h>
#include <Object3d.h>
#include "charcter/Collision/Collider.h"
class Player;

class Enemy : public Collider
{
public:
	// 初期化
	void Initialize(const Vector3& pos);
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();

	void Move();

	void Turning();
private:
	/// <summary>
/// モーション全体の初期化
/// </summary>
	void BehaviorInitialize();

	/// <summary>
	/// モーション全体の更新
	/// </summary>
	void BehaviorUpdate();

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInit();

	/// <summary>
	/// ノックバック初期化
	/// </summary>
	void BehaviorKnockBackInit();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// ノックバック更新
	/// </summary>
	void BehaviorKnockBackUpdate();

	void DeadUpdate();
public: // 構造体
	enum class EnemyBehavior {
		kRoot,
		kKnockBack,
	};
public: // アクセッサ
	Vector3 GetCenterPosition() const override;
	void OnCollision([[maybe_unused]] Collider* other) override;
	void SetPlayer(Player* player) { player_ = player; }

	bool IsDeadTriger();
private:
	std::unique_ptr<Object3d> object_ = nullptr;
	WorldTransform transform_;

	// 影用のオブジェクト
	std::unique_ptr<Object3d> shadeObject_;
	WorldTransform shadeTransform_;

	Vector3 vel_;
	float speed_;
	const Vector3 gravity = { 0.0f, -9.8f, 0.0f };
	const float deltaTime = 1.0f / 60.0f;
	const float kSpeed_ = 0.1f;
	bool onCollision_;

	float hp_ = 50.0f;
	bool isAlive = true;
	bool preIsAlive = true;


	// プレイヤーのポインタ
	Player* player_ = nullptr;

public:
	// 振る舞い
	EnemyBehavior behavior_ = EnemyBehavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<EnemyBehavior> behaviortRquest_ = std::nullopt;
};

