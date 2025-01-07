#include "GameCamera.h"

void GameCamera::Initialize()
{
	// カメラの生成
	gameCamera_ = std::make_shared<Camera>("GameCamera");
	debugCamera_ = std::make_shared<Camera>("DebugCamera");
	cameraManager_.AddCamera(gameCamera_);
	cameraManager_.AddCamera(debugCamera_);
	cameraManager_.SetActiveCamera(0);

	input_ = Input::GetInstance();
}

void GameCamera::Finalize()
{
}

void GameCamera::Update()
{
    // キー入力で回転角度を調整
    if (input_->PushKey(DIK_LEFT)) {
        orbitAngle_ -= 0.05f; // 左回転
    }
    if (input_->PushKey(DIK_RIGHT)) {
        orbitAngle_ += 0.05f; // 右回転
    }

    // プレイヤーを中心にカメラの位置を計算
    if (player_ != nullptr) {
        Vector3 playerPosition = player_->GetTranslate();

        float x = std::cos(orbitAngle_) * orbitRadius_;
        float z = std::sin(orbitAngle_) * orbitRadius_;
        Vector3 cameraPosition = playerPosition + Vector3(x, 8.0f, z); // 上方向のオフセットを4.0に設定

        // カメラの位置と向きを設定
        gameCamera_->SetTranslate(cameraPosition);

        // カメラの向きをプレイヤーに向ける
        Vector3 direction = Normalize(playerPosition - cameraPosition);
        float yaw = std::atan2(direction.x, direction.z);
        gameCamera_->SetRotate(Vector3(0.2f, yaw, 0.0f));

        //ImGui::Begin("Camera Manager");
        //ImGui::DragFloat3("normalPos", &cameraPosition.x, 0.01f);
        //ImGui::DragFloat("normalRotate", &yaw, 0.01f);
        //ImGui::End();
    }

    // カメラの更新処理
    cameraManager_.Update();
}
