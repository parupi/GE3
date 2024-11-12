#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include <Input.h>
class Player
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	WorldTransform transform_;
	Object3d* object_;
	// キーボード入力
	Input* input_ = nullptr;

public:
	void SetParent(const WorldTransform* parent);
};

