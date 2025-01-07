#pragma once
#include "charcter/player/PlayerState.h"
#include "Vector3.h"
#include "Vector2.h"
#include "GlobalVariables.h"

class FirstAttackState : public PlayerState {
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Exit(Player* player) override;
	void UpdateJson();
private:
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Vector3 startPos_{ 0.0f, 0.0f, 0.0f };
	Vector3 midPos_{ -0.5f, 1.0f, 3.0f };
	Vector3 endPos_{ -2.0f, 1.0f, 1.0f };
	Vector3 startRot_{ 1.56f, 0.0f, 0.0f };
	Vector3 midRot_{ 1.56f, 0.0f, 0.0f };
	Vector3 endRot_{ 1.56f, 0.0f, 0.0f };

	bool isNextAttack_ = false;

	float attackTimer_ = 0.0f;
	const float maxTime_ = global_->GetFloatValue("firstAttack", "time");
	const Vector2& kKnockBack_ = { global_->GetVector3Value("firstAttack", "KnockBack").x, global_->GetVector3Value("firstAttack", "KnockBack").y };
	Vector3 playerMove_ = global_->GetVector3Value("firstAttack", "playerMove"); // プレイヤーの移動量
};