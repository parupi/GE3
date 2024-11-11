#pragma once
#include <WorldTransform.h>
#include <Object3d.h>
#include <Rail.h>
#include <CameraManager.h>
class RailManager
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:
	CameraManager cameraManager_;
	std::shared_ptr<Camera> railCamera_;
	std::shared_ptr<Camera> debugCamera_;

	std::vector<Rail*> rails_;

	// スプライン曲線制御点
	std::vector<Vector3> controlPoints_ =
	{
		{0.0f, 0.0f, 0.0f},
		{10.0f, 10.0f, 5.0f},
		{20.0f, 0.0f, 0.0f},
		{30.0f, 10.0f, -5.0f},
		{40.0f, 0.0f,  0.0f},
		{50.0f, 10.0f, 5.0f},
	};

	float t_ = 0.0f;
	float speed_ = 0.05f;
	float deltaTime = 1.0f / 60.0f;

	const int kRailNum = 50;
};

