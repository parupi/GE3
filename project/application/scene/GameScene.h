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
#include <WorldTransform.h>
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
	WorldTransform worldTransform_;

	CameraManager cameraManager_;
	std::shared_ptr<Camera> normalCamera_;
	std::shared_ptr<Camera> bossCamera_;

	Object3d* object_;
	std::vector<Sprite*> sprites;

	Vector4 color1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 color2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	uint32_t soundHandle = 0u;
};

