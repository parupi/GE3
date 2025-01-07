#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerState.h"
#include <memory>
#include "Weapon.h"
#include "ParticleEmitter.h"

class Player {
public:
    void Initialize();
    void Finalize();
    void Update();
    void Draw();

    void ChangeState(std::unique_ptr<PlayerState> newState);
    void GlobalInitialize(const std::string cameraName);
    void ApplyGravity();
public:

    struct AttackData {
        Vector2 knockBack;
        float damage;
        bool isAttack = false;
    };
    AttackData attackData_;

public: // アクセッサ
    Vector3 GetTranslate() const { return transform_.translation_; }
    void SetTranslate(const Vector3& translate) { transform_.translation_ = translate; }
    Vector3 GetRotate() const { return transform_.rotation_; }
    void SetRotate(const Vector3& rotate) { transform_.rotation_ = rotate; }
    
    Weapon* GetWeapon() { return weapon_.get(); }
   
    void SetKnockBack(const Vector2& knockBack) { attackData_.knockBack = knockBack; }
    void SetAttackFlag(bool isAttack) { attackData_.isAttack = isAttack; }
    Vector2 GetKnockBack() { return attackData_.knockBack; }
    bool GetAttackFlag() { return attackData_.isAttack; }
    bool GetIsChangeState() { return isChangeState_; }

    void SetDamage(float damage) { attackData_.damage = damage; }
    float GetDamage() { return attackData_.damage; }

    Vector3 GetTranslateWeapon() const { return weapon_->GetTranslate(); }
    void SetTranslateWeapon(const Vector3& translate) { weapon_->SetTranslate(translate); }
    Vector3 GetRotateWeapon() const { return weapon_->GetRotate(); }
    void SetRotateWeapon(const Vector3& rotate) { weapon_->SetRotate(rotate); }

    Vector3 GetCameraPos() { return camera_->GetTranslate(); }
    void SetCamera(Camera* camera) { camera_ = camera; }

    Input* GetInput() const { return input_; }
    Vector3 GetCameraForward() const;
    Vector3 GetCameraRight() const;

    Vector3 GetFrontDirection() const;
private:
    GlobalVariables* global_ = GlobalVariables::GetInstance();

    //  武器の生成
    std::unique_ptr<Weapon> weapon_;
    // オブジェクトの生成
    std::unique_ptr<Object3d> object_;
    WorldTransform transform_;
    Input* input_ = nullptr;
    Camera* camera_ = nullptr;
    bool isChangeState_ = false;

    // 影用のオブジェクト
    std::unique_ptr<Object3d> shadeObject_;
    WorldTransform shadeTransform_;

    std::unique_ptr<PlayerState> currentState_;

    Vector3 vel_{};
    const Vector3 gravity = { 0.0f, -9.8f, 0.0f };
    const float deltaTime = 1.0f / 60.0f;

    std::unique_ptr<ParticleManager> particleManager_;
    std::unique_ptr<ParticleEmitter> emitter_;
};
