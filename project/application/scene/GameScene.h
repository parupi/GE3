#pragma once
#include <Object3d.h>
#include <Sprite.h>
#include <Model.h>
#include <vector>
#include <BaseScene.h>
#include <memory>
#include <Audio.h>
#include "camera/GameCamera.h"
#include "charcter/player/Player.h"
#include <charcter/Collision/CollisionManager.h>
#include "object/Ground.h"
#include "object/Skydome.h"
#include <LightManager.h>
#include <charcter/enemy/EnemyManager.h>

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

	void CheckAllCollisions();

private:
	// 衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<GameCamera> gameCamera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Skydome> sphere_;

	LightManager* lightManager_ = LightManager::GetInstance();

	std::unique_ptr<Sprite> titleUI_;
};

