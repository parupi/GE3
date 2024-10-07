#pragma once
#include <BaseScene.h>
class SceneManager
{
private:
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = default;
	SceneManager& operator=(SceneManager&) = default;
public:
	// シングルトンインスタンスの取得
	static SceneManager* GetInstance();
	// 次のシーンを予約する
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	// 実行中のシーン
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;
};