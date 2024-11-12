#pragma once
#include "Rail.h"
#include <Vector3.h>
#include <Quaternion.h>
class RailManager
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();
private:
	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_ = {
		{0.0f, 0.0f, 0.0f},
		{20.0f, 0.0f, 5.0f},
		{40.0f, 5.0f, 10.0f},
		{60.0f, 7.0f, 20.0f},
		{80.0f, 8.0f, 15.0f},
		{100.0f, 10.0f, 20.0f},
		{120.0f, 8.0f, 15.0f},
		{140.0f, 6.0f, 10.0f},
		{160.0f, 4.0f, 5.0f},
		{180.0f, 7.0f, 0.0f},
		{200.0f, 8.0f, -5.0f},
		{220.0f, 10.0f, -1.0f},
	};

	std::vector<Rail*> rails_;
	const int kRailNum = 150;

	float playerMove = 0.0f;
	float playerSpeed = 1.0f / 3600.0f;

	Vector3 cameraTranslate;
	Vector3 cameraRotate;
	Vector3 playerTranslate;
private:
	void CreateRail(const std::vector<Vector3>& startPoints, const std::vector<Vector3>& endPoints);

public:
	const Vector3 GetCameraTranslate() const { return cameraTranslate; }
	const Vector3 GetCameraRotate() const { return cameraRotate; }
	const Vector3 GetOffset() const { return playerTranslate; }
	float GetTimer() const { return playerMove; }
};

