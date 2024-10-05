#pragma once
#include <Object3d.h>
#include <Sprite.h>
#include <Model.h>
#include <vector>
#include <Camera.h>
class GameScene
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
	Camera* camera = nullptr;

	std::vector<Object3d*> objects;
	std::vector<Model*> models;
	std::vector<Sprite*> sprites;
};

