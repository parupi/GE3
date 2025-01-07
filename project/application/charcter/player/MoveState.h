#pragma once
#include "PlayerState.h"

class MoveState : public PlayerState {
public:
    void Enter(Player* player) override;
    void Update(Player* player) override;
    void Exit(Player* player) override;
private:
    void Move(Player* player);
private:
    const float playerSpeed = 0.5f;


};