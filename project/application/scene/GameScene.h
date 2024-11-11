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
#include <RailManager.h>
#include <LightManager.h>
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


	std::unique_ptr<RailManager> railManager_;
	std::unique_ptr<LightManager> lightManager_;

};

