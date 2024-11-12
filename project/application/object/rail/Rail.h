#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
class Rail
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

public: // セッター // ゲッター //
	void SetTranslate(const Vector3 translate) { transform_.translation_ = translate; }
	void SetRotate(const Vector3 rotate) { transform_.rotation_ = rotate; }
	void SetScale(const Vector3 scale) { transform_.scale_ = scale; }

};

