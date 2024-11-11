#include "RailManager.h"
#include <imgui.h>

void RailManager::Initialize()
{
	for (size_t i = 0; i < kRailNum; i++) {
		Rail* rail = new Rail();
		rail->Initialize();
		rail->SetScale({ 1.0f, 1.0f, 1.0f });
		rails_.push_back(rail);
	}

	// カメラの生成
	railCamera_ = std::make_shared<Camera>();
	debugCamera_ = std::make_shared<Camera>();
	cameraManager_.AddCamera(railCamera_);
	cameraManager_.AddCamera(debugCamera_);
	cameraManager_.SetActiveCamera(1);
	debugCamera_->SetTranslate(Vector3{ 15.0f, 25.0f, -50.0f });
	//railCamera_->SetTranslate(Vector3{ 15.0f, 25.0f, -50.0f });

}

void RailManager::Finalize()
{

}

void RailManager::Update()
{
	ImGui::Begin("controlPoint");
	ImGui::DragFloat3("point1", &controlPoints_[0].x, 0.01f);
	ImGui::DragFloat3("point2", &controlPoints_[1].x, 0.01f);
	ImGui::DragFloat3("point3", &controlPoints_[2].x, 0.01f);
	ImGui::DragFloat3("point4", &controlPoints_[3].x, 0.01f);
	ImGui::DragFloat3("point5", &controlPoints_[4].x, 0.01f);
	ImGui::DragFloat3("point6", &controlPoints_[5].x, 0.01f);
	ImGui::End();

	cameraManager_.Update();

	Vector3 debugCameraPos = debugCamera_->GetTranslate();
	Vector3 debugCameraRotate = debugCamera_->GetRotate();

	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &debugCameraPos.x, 0.01f);
	ImGui::DragFloat3("Rotation", &debugCameraRotate.x, 0.01f);

	if (ImGui::Button("railCamera")) {
		cameraManager_.SetActiveCamera(0);
	}
	else if (ImGui::Button("debugCamera")) {
		cameraManager_.SetActiveCamera(1);
	}

	ImGui::End();

	debugCamera_->SetTranslate(debugCameraPos);
	debugCamera_->SetRotate(debugCameraRotate);

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsUpdating;

	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < kRailNum + 1; i++) {
		float t = 1.0f / kRailNum * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsUpdating.push_back(pos);
	}
	//先頭から2点ずつ取り出してライン描画
	for (size_t i = 0; i < pointsUpdating.size() - 1; i++) {
		// 2点間のベクトルを取得
		const Vector3& startPoint = pointsUpdating[i];
		const Vector3& endPoint = pointsUpdating[i + 1];

		// 現在の方向ベクトルを計算
		Vector3 direction = Normalize(endPoint - startPoint);

		// 前の方向ベクトルを取得し、平均方向を計算
		Vector3 prevDirection = (i > 0) ? Normalize(pointsUpdating[i] - pointsUpdating[i - 1]) : direction;
		Vector3 smoothDirection = Normalize((direction + prevDirection) * 0.5f);

		// 回転角度を求める
		Vector3 rotation;
		rotation.x = std::atan2(smoothDirection.y, std::sqrt(smoothDirection.x * smoothDirection.x + smoothDirection.z * smoothDirection.z));
		rotation.y = std::atan2(smoothDirection.x, smoothDirection.z);
		rotation.z = 0.0f;

		// レールオブジェクトの配置と回転を設定
		rails_[i]->SetTranslation(startPoint);
		rails_[i]->SetRotation(rotation); // Vector3 の回転を直接設定
	}

	// カメラの更新
	Vector3 cameraPos = CatmullRomPosition(controlPoints_, t_);
	Vector3 lookAt = CatmullRomPosition(controlPoints_, t_ + 0.01f); // 少し先の位置を注視点とする

	// 進行方向を求める
	Vector3 direction = Normalize(lookAt - cameraPos);

	// カメラの回転を計算（ロールは水平に固定）
	Vector3 cameraRotation = CalculateCameraRotationFromDirection(direction, 0.0f); // ロールは0度に固定

	railCamera_->SetTranslate(cameraPos);
	railCamera_->SetRotate(cameraRotation);

	if (t_ <= 1.0f) {
		t_ += speed_ * deltaTime;
	}
	for (size_t i = 0; i < rails_.size(); ++i) {
		rails_[i]->Update();
	}

	Vector3 axis = Normalize({ 1.0f, 1.0f, 1.0f });
	float angle = 0.44f;
	Matrix4x4 rotationMatrix = MakeRotationAxisAngle(axis, angle);
	MatrixPrintImGui(rotationMatrix);

}

void RailManager::Draw()
{
	for (size_t i = 0; i < rails_.size(); ++i) {
		rails_[i]->Draw();
	}
}
