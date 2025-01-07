#include "FirstAttackState.h"
#include "SecondAttackState.h"
#include <charcter/player/Player.h>
#include <charcter/player/IdleState.h>

void FirstAttackState::Enter(Player* player) {
	// 攻撃状態への遷移時の初期化処理
	attackTimer_ = maxTime_;
	player->SetAttackFlag(true);
	player->SetKnockBack(kKnockBack_);
	player->SetDamage(1.0f);
}

void FirstAttackState::Update(Player* player) {
	UpdateJson();
	// 攻撃タイマーを更新
	attackTimer_ -= 1.0f / 60.0f;

	float normalizedTime = (maxTime_ - attackTimer_) / maxTime_;

	Vector3 weaponPos, weaponRot;

	if (normalizedTime <= 0.5f) {
		// 前半: 初期位置から中間点への補間
		float t = normalizedTime * 2.0f;
		weaponPos = Lerp(startPos_, midPos_, t);
		weaponRot = Lerp(startRot_, midRot_, t);
	}
	else {
		// 後半: 中間点から終点への補間
		float t = (normalizedTime - 0.5f) * 2.0f;
		weaponPos = Lerp(midPos_, endPos_, t);
		weaponRot = Lerp(midRot_, endRot_, t);
	}

	// プレイヤーの向いている方向を取得
	Vector3 frontDirection = Normalize(player->GetFrontDirection());

	// プレイヤーの移動を計算
	Vector3 playerCurrentMove = frontDirection * (Length(playerMove_) * normalizedTime);
	Vector3 newPlayerPos = player->GetTranslate() + playerCurrentMove;

	// プレイヤーの位置と武器の位置・回転を設定
	player->SetTranslateWeapon(weaponPos);
	player->SetRotateWeapon(weaponRot);
	player->SetTranslate(newPlayerPos);

	if (player->GetInput()->TriggerKey(DIK_SPACE)) {
		isNextAttack_ = true;
	}

	if (attackTimer_ <= 0.0f) {
		if (isNextAttack_) {
			player->ChangeState(std::make_unique<SecondAttackState>());
		}
		else {
			player->ChangeState(std::make_unique<IdleState>());
		}
	}
}

void FirstAttackState::Exit(Player* player) {
	// 攻撃状態の終了処理
	player->SetAttackFlag(false);
	isNextAttack_ = false;
}

void FirstAttackState::UpdateJson()
{
	startPos_ = global_->GetVector3Value("firstAttack", "startPos");
	midPos_ = global_->GetVector3Value("firstAttack", "midPos");
	endPos_ = global_->GetVector3Value("firstAttack", "endPos");
	startRot_ = global_->GetVector3Value("firstAttack", "startRot");
	midRot_ = global_->GetVector3Value("firstAttack", "midRot");
	endRot_ = global_->GetVector3Value("firstAttack", "endRot");
}
