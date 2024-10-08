#pragma once
#include <Object3d.h>
#include <Sprite.h>
#include <Model.h>
#include <vector>
#include <Camera.h>
#include <BaseScene.h>
#include <memory>
#include <Audio.h>
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
	std::unique_ptr<Camera> camera = nullptr;

	std::vector<Object3d*> objects;
	std::vector<Model*> models;
	std::vector<Sprite*> sprites;

	uint32_t soundHandle = 0u;
};

