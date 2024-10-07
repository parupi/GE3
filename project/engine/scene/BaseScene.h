#pragma once
class SceneManager;

class BaseScene
{
public:
	// デストラクタ
	virtual ~BaseScene() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 終了
	virtual void Finalize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;

public:
	// シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	virtual void SetSceneManager(SceneManager* sceneManager){sceneManager_ = sceneManager;}

};

