#pragma once
#include "charcter/player/PlayerState.h"
#include "Vector3.h"
#include "Vector2.h"
#include "GlobalVariables.h"

class FourthAttackState : public PlayerState {
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Exit(Player* player) override;
	void UpdateJson();
private:
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Vector3 startPos_{};
	Vector3 midPos_{};
	Vector3 endPos_{};
	Vector3 startRot_{};
	Vector3 midRot_{};
	Vector3 endRot_{};

	bool isNextAttack_ = false;

	float attackTimer_ = 0.0f;
	const float maxTime_ = global_->GetFloatValue("fourthAttack", "time");
	const Vector2& kKnockBack_ = { global_->GetVector3Value("fourthAttack", "KnockBack").x, global_->GetVector3Value("fourthAttack", "KnockBack").y };
	Vector3 playerMove_ = global_->GetVector3Value("fourthAttack", "playerMove"); // プレイヤーの移動量
};