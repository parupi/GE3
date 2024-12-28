#include "IdleState.h"
#include "MoveState.h"
#include "Player.h"

void IdleState::Enter(Player* player) {
    // 待機状態への遷移時の初期化処理
}

void IdleState::Update(Player* player) {
    if (player->GetInput()->PushKey(DIK_W) || player->GetInput()->PushKey(DIK_A) ||
        player->GetInput()->PushKey(DIK_S) || player->GetInput()->PushKey(DIK_D)) {
        player->ChangeState(std::make_unique<MoveState>());
    }
}

void IdleState::Exit(Player* player) {
    // 待機状態の終了処理
}
