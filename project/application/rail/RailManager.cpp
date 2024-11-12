#include "RailManager.h"

void RailManager::Initialize()
{
	for (size_t i = 0; i < kRailNum; i++) {
		Rail* rail = new Rail();
		rail->Initialize();
		rails_.push_back(rail);
	}
}

void RailManager::Update()
{
	ImGui::Begin("controlPoint");
	ImGui::DragFloat3("controlPoint0", &controlPoints_[0].x, 0.01f);
	ImGui::DragFloat3("controlPoint1", &controlPoints_[1].x, 0.01f);
	ImGui::DragFloat3("controlPoint2", &controlPoints_[2].x, 0.01f);
	ImGui::DragFloat3("controlPoint3", &controlPoints_[3].x, 0.01f);
	ImGui::DragFloat3("controlPoint4", &controlPoints_[4].x, 0.01f);
	ImGui::DragFloat3("controlPoint5", &controlPoints_[5].x, 0.01f);
	ImGui::DragFloat3("controlPoint6", &controlPoints_[6].x, 0.01f);
	ImGui::DragFloat3("controlPoint7", &controlPoints_[7].x, 0.01f);
	//ImGui::DragFloat3("controlPoint8", &controlPoints_[8].x, 0.01f);
	ImGui::End();

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsUpdating;
	std::vector<Vector3> startPos; // 各セグメントの始点を格納
	std::vector<Vector3> endPos;   // 各セグメントの終点を格納
	Vector3 lineVector{};

	// 各レールセグメントの位置と回転を計算
	for (size_t i = 0; i < kRailNum + 1; i++) {
		float t = 1.0f / kRailNum * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		pointsUpdating.push_back(pos);
	}
	//先頭から2点ずつ取り出し始点と終点の計算
	for (size_t i = 0; i < pointsUpdating.size() - 1; i++) {
		// 先頭から2点ずつ取り出す
		startPos.push_back(pointsUpdating[i]);
		endPos.push_back(pointsUpdating[i + 1]);
		lineVector = pointsUpdating[i + 1] - pointsUpdating[i];
	}
	CreateRail(startPos, endPos);

	// カメラの位置と向きを更新
	playerMove += playerSpeed;
	if (playerMove >= 1.0f) {
		playerMove = 0.0f;
	}

	// 現在のカメラ位置をCatmull-Romスプラインで取得
	cameraTranslate = CatmullRomPosition(controlPoints_, playerMove);
	// 少し先の位置を計算し、カメラの進行方向ベクトルを求める
	float nextT = std::min(playerMove + 0.01f, 1.0f);
	Vector3 nextPos = CatmullRomPosition(controlPoints_, nextT);

	// カメラの進行方向を計算し、正規化
	Vector3 direction = Normalize(nextPos - cameraTranslate);
	// 進行方向に垂直な上方向を計算（常にY軸上方向を基準にする）
	Vector3 up(0.0f, 1.0f, 0.0f);
	Vector3 offsetUp = Cross(direction, Cross(up, direction)); // 進行方向と上ベクトルから垂直上方向を計算
	offsetUp = Normalize(offsetUp); // 正規化して長さを1にする
	// 上方向にオフセット（例として0.5の高さ分）
	cameraTranslate += offsetUp * 0.5f;

	// カメラが X 軸方向 (1, 0, 0) を向いていると仮定して、目的の方向へ回転するクォータニオンを計算
	Quaternion rotationQuat = SetFromTo(Vector3(1.0f, 0.0f, 0.0f), direction);
	// クォータニオンをオイラー角に変換し、微調整を追加
	Vector3 eulerRotation = ToEulerAngles(rotationQuat);

	eulerRotation.y += 1.56f;
	cameraRotate = eulerRotation;
	cameraTranslate += {0.0f, 0.5f, 0.0f};

	for (size_t i = 0; i < kRailNum; i++) {
		rails_[i]->Update();
	}
}

void RailManager::Draw()
{
	for (size_t i = 0; i < kRailNum; i++) {
		rails_[i]->Draw();
	}
}

void RailManager::CreateRail(const std::vector<Vector3>& startPoses, const std::vector<Vector3>& endPoses)
{
	for (size_t i = 0; i < startPoses.size(); ++i) {
		const Vector3& startPos = startPoses[i];
		const Vector3& endPos = endPoses[i];

		// 始点と中点の中間点を求める
		Vector3 midPos = (startPos + endPos) * 0.5f;
		rails_[i]->SetTranslate(midPos);

		// 始点と終点の距離からスケールを計算
		float length = Length(endPos - startPos);
		rails_[i]->SetScale({ length/2.0f, 1.0f, 1.0f });

		// クォータニオンで回転の計算（始点から終点への回転を計算）
		Quaternion rotationQuat;
		rotationQuat = SetFromTo(Vector3(1.0f, 0.0f, 0.0f), Normalize(endPos - startPos)); // X軸方向から目的の方向への回転を計算
		Vector3 eulerRotation = ToEulerAngles(rotationQuat); // オイラー角に変換して設定
		eulerRotation.x += 1.56f;
		rails_[i]->SetRotate(eulerRotation);
	}
}