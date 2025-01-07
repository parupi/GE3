#include "IdleState.h"
#include "MoveState.h"
#include "Player.h"
#include "charcter/player/attack/FirstAttackState.h"

void IdleState::Enter(Player* player) {
    // 待機状態への遷移時の初期化処理
    player->SetTranslateWeapon({ 1.0f, 2.0f, 1.0f });
    player->SetRotateWeapon({ 0.0f, 1.56f, 0.0f });
}

void IdleState::Update(Player* player) {
    if (player->GetInput()->PushKey(DIK_W) || player->GetInput()->PushKey(DIK_A) ||
        player->GetInput()->PushKey(DIK_S) || player->GetInput()->PushKey(DIK_D)) {
        player->ChangeState(std::make_unique<MoveState>());
    }

    if (player->GetInput()->TriggerKey(DIK_SPACE)) {
        player->ChangeState(std::make_unique<FirstAttackState>());
    }
}

void IdleState::Exit(Player* player) {
    // 待機状態の終了処理
}
