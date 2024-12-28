#pragma once
#include <WorldTransform.h>
#include <Object3d.h>
class Enemy
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
	std::unique_ptr<Object3d> object_ = nullptr;
	WorldTransform transform_;

	

};

