#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerState.h"
#include <memory>

class Player {
public:
    void Initialize();
    void Finalize();
    void Update();
    void Draw();

    void ChangeState(std::unique_ptr<PlayerState> newState);

    Vector3 GetTranslate() const { return transform_.translation_; }
    void SetTranslate(const Vector3& translate) { transform_.translation_ = translate; }
    Vector3 GetRotate() const { return transform_.rotation_; }
    void SetRotate(const Vector3& rotate) { transform_.rotation_ = rotate; }
    void SetCamera(Camera* camera) { camera_ = camera; }

    Input* GetInput() const { return input_; }
    Vector3 GetCameraForward() const;
    Vector3 GetCameraRight() const;
private:
    std::unique_ptr<Object3d> object_;
    WorldTransform transform_;
    Input* input_ = nullptr;
    Camera* camera_ = nullptr;

    std::unique_ptr<PlayerState> currentState_;
};
