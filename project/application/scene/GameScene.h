#pragma once
#include <Object3d.h>
#include <Sprite.h>
#include <Model.h>
#include <vector>
#include <Camera.h>
#include <BaseScene.h>
#include <memory>
#include <Audio.h>
#include <CameraManager.h>
#include "RailManager.h"
#include "LightManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Sphere.h"
class GameScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
private:
	// 衝突判定と応答
	void CheckAllCollision();

private:
	CameraManager cameraManager_;
	std::shared_ptr<Camera> normalCamera_;
	std::shared_ptr<Camera> railCamera_;
	WorldTransform railCameraTransform_;

	RailManager* railManager_;
	Player* player_;
	EnemyManager* enemyManager_;
	
	LightManager* lightManager_;

	// 天球
	Sphere* sphere_ = nullptr;

	float score = 0.0f;
};

