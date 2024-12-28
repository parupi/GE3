#pragma once
#include <CameraManager.h>
#include <memory>
#include <charcter/player/Player.h>
class GameCamera
{
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();

private:
	CameraManager cameraManager_;
	std::shared_ptr<Camera> gameCamera_;
	std::shared_ptr<Camera> debugCamera_;

private:
	float orbitAngle_ = 0.0f; // カメラの現在の回転角度（ラジアン）
	float orbitRadius_ = 30.0f; // プレイヤーからの距離

	Input* input_ = nullptr; // 入力デバイス
	Player* player_ = nullptr;
public: // アクセッサ
	void SetPlayer(Player* player) { player_ = player; }
	Camera* GetGameCamera() { return gameCamera_.get(); }
};

