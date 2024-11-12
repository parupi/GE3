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

private:
	// スプライン曲線制御点
	//std::vector<Vector3> controlPoints_ = {
	//	{0.0f, 0.0f, -3.0f},
	//	{20.0f, 0.0f, -2.0f},
	//	{40.0f, 5.0f, -1.0f},
	//	{60.0f, 7.0f, 0.0f},
	//	{80.0f, 8.0f, 0.0f},
	//	{100.0f, 10.0f, 1.0f},
	//	{120.0f, 8.0f, 2.0f},
	//	{140.0f, 7.0f, 2.0f},
	//	//{160.0f, 5.0f, 3.0f},
	//	//{180.0f, 3.0f, 4.0f},
	//};

	//std::vector<Vector3> controlPoints_ = {
	//	{0, 0, 0},        // スタート地点
	//	{10, 5, 10},  // 上昇
	//	{20, 10, 20},  // 下り
	//	{30, 15, 10},  // 再度上昇
	//	{40, 0, 0},   // 急下降
	//	{50, 5, -10},  // 緩やかな上昇
	//	{60, 10, -20}, // 高く登る
	//	{70, 5, 0},      // 下りながら進む
	//	{80, -5, 15}, // 急降下
	//	{90, 0, 25},  // ゆっくり上昇
	//	{100, 0, 10},   // 平坦に進む
	//	{110, -5, 5}, // さらに下降して終わる
	//};

	std::vector<Vector3> controlPoints_ = {
		{0, 0, 0},        // スタート地点
		{100, 150, 100},  // 上昇
		{200, 500, 200},  // 下り
		{300, 200, 100},  // 再度上昇
		{400, -100, 0},   // 急下降
		{500, 50, -100},  // 緩やかな上昇
		{600, 300, -200}, // 高く登る
		{700, 0, 0},      // 下りながら進む
		{800, -150, 150}, // 急降下
		{900, 100, 250},  // ゆっくり上昇
		{1000, 0, 100},   // 平坦に進む
		{1100, -200, 50}, // さらに下降して終わる
	};

	std::vector<Rail*> rails_;
	const int kRailNum = 500;

	float playerMove = 0.0f;
	float playerSpeed = 1.0f / 3600;

	Vector3 cameraTranslate;
	Vector3 cameraRotate;
private:
	void CreateRail(const std::vector<Vector3>& startPoints, const std::vector<Vector3>& endPoints);

public:
	const Vector3 GetCameraTranslate() const { return cameraTranslate; }
	const Vector3 GetCameraRotate() const { return cameraRotate; }
};

