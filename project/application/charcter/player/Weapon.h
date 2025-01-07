#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include "charcter/Collision/Collider.h"
class Weapon : public Collider
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();


	Vector3 GetCenterPosition() const override;
	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetTranslate() const { return transform_.translation_; }
	void SetTranslate(const Vector3& translate) { transform_.translation_ = translate; }
	Vector3 GetRotate() const { return transform_.rotation_; }
	void SetRotate(const Vector3& rotate) { transform_.rotation_ = rotate; }

private:

	std::unique_ptr<Object3d> object_ = nullptr;
	WorldTransform transform_;

public: // アクセッサ
	void SetParent(const WorldTransform& worldTransform) { transform_.parent_ = &worldTransform; }



};

