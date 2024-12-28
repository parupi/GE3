#include "AttackState.h"
#include "IdleState.h"
#include "Player.h"

void AttackState::Enter(Player* player) {
    // 攻撃状態への遷移時の初期化処理
}

void AttackState::Update(Player* player) {
    // 攻撃処理を追加
    if (!player->GetInput()->PushKey(DIK_SPACE)) {
        player->ChangeState(std::make_unique<IdleState>());
    }
}

void AttackState::Exit(Player* player) {
    // 攻撃状態の終了処理
}
