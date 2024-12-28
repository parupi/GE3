#include "Player.h"
#include "IdleState.h"

void Player::Initialize() {
    object_ = std::make_unique<Object3d>();
    object_->Initialize("plane.obj");

    transform_.Initialize();

    input_ = Input::GetInstance();
    currentState_ = std::make_unique<IdleState>();
    currentState_->Enter(this);
}

void Player::Finalize() {
    currentState_->Exit(this);
}

void Player::Update() {
    currentState_->Update(this);
    transform_.TransferMatrix();
}

void Player::Draw() {
    object_->Draw(transform_);
}

void Player::ChangeState(std::unique_ptr<PlayerState> newState) {
    currentState_->Exit(this);
    currentState_ = std::move(newState);
    currentState_->Enter(this);
}

Vector3 Player::GetCameraForward() const {
    float yaw = camera_->GetRotate().y;
    return Vector3(std::sin(yaw), 0.0f, std::cos(yaw));
}

Vector3 Player::GetCameraRight() const {
    float yaw = camera_->GetRotate().y;
    return Vector3(std::cos(yaw), 0.0f, -std::sin(yaw));
}