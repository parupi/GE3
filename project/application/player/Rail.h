#pragma once
#include <Object3d.h>
#include <WorldTransform.h>
class Rail
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
	WorldTransform transform_;
	Object3d* object_;

public: // セッター // ゲッター //
	// 平行移動
	const Vector3& GetTranslation() const { return transform_.translation_; }
	void SetTranslation(const Vector3& translation) { transform_.translation_ = translation; }
	// 回転
	const Vector3& GetRotation() const { return transform_.rotation_; }
	void SetRotation(const Vector3& rotation) { transform_.rotation_ = rotation; }
	// 拡縮
	const Vector3& GetScale() const { return transform_.scale_; }
	void SetScale(const Vector3& scale) { transform_.scale_ = scale; }
};

