#include "SixthAttackState.h"
#include "charcter/player/IdleState.h"
#include "charcter/player/Player.h"

void SixthAttackState::Enter(Player* player) {
	// 攻撃状態への遷移時の初期化処理
	attackTimer_ = maxTime_;
	player->SetAttackFlag(true);
	player->SetKnockBack(kKnockBack_);
	player->SetDamage(10.0f);
}

void SixthAttackState::Update(Player* player) {
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

	// プレイヤーの移動を計算（y座標は固定）
	Vector3 playerCurrentMove = frontDirection * Length(playerMove_) * normalizedTime;
	playerCurrentMove.y = playerMove_.y; // y座標をそのまま設定

	// 新しいプレイヤーの位置を計算
	Vector3 newPlayerPos = player->GetTranslate() + playerCurrentMove;


	// プレイヤーの位置と武器の位置・回転を設定
	player->SetTranslateWeapon(weaponPos);
	player->SetRotateWeapon(weaponRot);
	player->SetTranslate(newPlayerPos);

	if (attackTimer_ <= 0.0f) {
		player->ChangeState(std::make_unique<IdleState>());
	}
}

void SixthAttackState::Exit(Player* player) {
	// 攻撃状態の終了処理
	player->SetAttackFlag(false);
}

void SixthAttackState::UpdateJson()
{
	startPos_ = global_->GetVector3Value("sixthAttack", "startPos");
	midPos_ = global_->GetVector3Value("sixthAttack", "midPos");
	endPos_ = global_->GetVector3Value("sixthAttack", "endPos");
	startRot_ = global_->GetVector3Value("sixthAttack", "startRot");
	midRot_ = global_->GetVector3Value("sixthAttack", "midRot");
	endRot_ = global_->GetVector3Value("sixthAttack", "endRot");
}
