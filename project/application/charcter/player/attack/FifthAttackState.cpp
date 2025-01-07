#include "FifthAttackState.h"
#include "charcter/player/IdleState.h"
#include "charcter/player/Player.h"
#include "SixthAttackState.h"

void FifthAttackState::Enter(Player* player) {
    // 攻撃状態への遷移時の初期化処理
    attackTimer_ = maxTime_;
    player->SetAttackFlag(true);
    player->SetKnockBack(kKnockBack_);
    player->SetDamage(5.0f);
}

void FifthAttackState::Update(Player* player) {
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

    if (player->GetInput()->TriggerKey(DIK_SPACE)) {
        isNextAttack_ = true;
    }

    if (attackTimer_ <= 0.0f) {
        if (isNextAttack_) {
            player->ChangeState(std::make_unique<SixthAttackState>());
        }
        else {
            player->ChangeState(std::make_unique<IdleState>());
        }
    }
}

void FifthAttackState::Exit(Player* player) {
    // 攻撃状態の終了処理
    player->SetAttackFlag(false);
}

void FifthAttackState::UpdateJson()
{
    startPos_ = global_->GetVector3Value("fifthAttack", "startPos");
    midPos_ = global_->GetVector3Value("fifthAttack", "midPos");
    endPos_ = global_->GetVector3Value("fifthAttack", "endPos");
    startRot_ = global_->GetVector3Value("fifthAttack", "startRot");
    midRot_ = global_->GetVector3Value("fifthAttack", "midRot");
    endRot_ = global_->GetVector3Value("fifthAttack", "endRot");
}
