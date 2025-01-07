#include "MoveState.h"
#include "IdleState.h"
#include "Player.h"
#include "attack/FirstAttackState.h"

void MoveState::Enter(Player* player) {
    // 移動状態への遷移時の初期化処理
}

void MoveState::Update(Player* player) {
    Move(player);

    if (!(player->GetInput()->PushKey(DIK_W) || player->GetInput()->PushKey(DIK_A) ||
        player->GetInput()->PushKey(DIK_S) || player->GetInput()->PushKey(DIK_D))) {
        player->ChangeState(std::make_unique<IdleState>());
    }


}

void MoveState::Exit(Player* player) {
    // 移動状態の終了処理
}

void MoveState::Move(Player* player) {
    Vector3 velocity = {};
    Vector3 inputDirection = {};

    if (player->GetInput()->PushKey(DIK_W)) {
        inputDirection.z += 1.0f;
    }
    if (player->GetInput()->PushKey(DIK_S)) {
        inputDirection.z -= 1.0f;
    }
    if (player->GetInput()->PushKey(DIK_A)) {
        inputDirection.x -= 1.0f;
    }
    if (player->GetInput()->PushKey(DIK_D)) {
        inputDirection.x += 1.0f;
    }

    Vector3 forward = player->GetCameraForward();
    Vector3 right = player->GetCameraRight();

    velocity = forward * inputDirection.z + right * inputDirection.x;

    if (Length(velocity) > 0.0f) {
        Vector3 direction = Normalize(velocity);
        Vector3 currentForward = { 0.0f, 0.0f, 1.0f };
        Quaternion rotation = FromToRotation(currentForward, direction);
        player->SetRotate(QuaternionToEuler(rotation));
    }

    Vector3 newTranslation = player->GetTranslate() + velocity * playerSpeed;
    player->SetTranslate(newTranslation);

    if (player->GetInput()->TriggerKey(DIK_SPACE)) {
        player->ChangeState(std::make_unique<FirstAttackState>());
    }
}