#include "Weapon.h"

void Weapon::Initialize()
{
	object_ = std::make_unique<Object3d>();
	object_->Initialize("sord/sord.obj");

	transform_.Initialize();
	transform_.translation_ = { 1.0f, 1.5f, 0.0f };
	transform_.scale_ = { 0.4f, 0.4f, 0.4f };

	Collider::Initialize();
	// 種別のIDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
}

void Weapon::Update()
{
	//ImGui::Begin("Weapon");
	//ImGui::DragFloat3("translate", &transform_.translation_.x, 0.01f);
	//ImGui::DragFloat3("rotation", &transform_.rotation_.x, 0.01f);
	//ImGui::DragFloat3("scale", &transform_.scale_.x, 0.01f);
	//ImGui::End();


	transform_.TransferMatrix();
}

void Weapon::Draw()
{
	object_->Draw(transform_);
}

Vector3 Weapon::GetCenterPosition() const
{
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, transform_.matWorld_);
	return worldPos;
}

void Weapon::OnCollision(Collider* other)
{

}
