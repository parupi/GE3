#include <GameScene.h>
#include <TextureManager.h>
#include <ModelManager.h>
#include <ParticleManager.h>

void GameScene::Initialize()
{
	// カメラの生成
	camera = std::make_unique<Camera>();
	Object3dManager::GetInstance()->SetDefaultCamera(camera.get());

	// Textureのロード
	TextureManager::GetInstance()->LoadTexture("resource/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resource/monsterBall.png");
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	// パーティクルのグループを生成
	//ParticleManager::GetInstance()->CreateParticleGroup("circle", "resource/uvChecker.png");
	// パーティクルにカメラをセットする
	//ParticleManager::GetInstance()->SetCamera(camera);
	Audio::GetInstance()->SoundLoadWave("resource/sound/fanfare.wav");
	Audio::GetInstance()->SoundPlayWave(Audio::GetInstance()->GetSoundData()["resource/sound/fanfare.wav"]);

	// Spriteの初期化
	for (uint32_t i = 0; i < 2; ++i) {
		
		Sprite* sprite = new Sprite();
		if (i == 1) {
			sprite->Initialize(SpriteManager::GetInstance(), "resource/monsterBall.png");
		}
		else {
			sprite->Initialize(SpriteManager::GetInstance(), "resource/uvChecker.png");
		}

		Vector2 initialPosition = Vector2{ 100.0f, 100.0f }; // 各スプライトを100ピクセルずつ右にずらして配置
		sprite->SetPosition(initialPosition);

		sprites.push_back(sprite);
	}

	for (int i = 0; i < 2; ++i) {

		// Modelの初期化
		Model* model = new Model();
		if (i == 1) {
			model->Initialize(ModelManager::GetInstance()->GetModelLoader(), "resource", "axis.obj");
		}
		else {
			model->Initialize(ModelManager::GetInstance()->GetModelLoader(), "resource", "plane.obj");
		}
		models.push_back(model);

		// Object3dの初期化
		Object3d* object = new Object3d();
		object->Initialize(Object3dManager::GetInstance());
		object->SetModel(model);
		objects.push_back(object);
	}
	
	// それぞれのObject3dに位置や回転を設定
	models[0]->SetPosition({ -2.0f, 0.0f, 0.0f });
	models[0]->SetRotation({ 0.0f, 0.0f, 0.0f });

	models[1]->SetPosition({ 2.0f, 0.0f, 0.0f });
	models[1]->SetRotation({ 0.0f, 45.0f, 0.0f });
}

void GameScene::Finalize()
{
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	for (Object3d* object : objects) {
		delete object;
		object = nullptr;
	}
	for (Model* model : models) {
		delete model;
		model = nullptr;
	}
}

void GameScene::Update()
{
	camera->Update();

	// ゲームの処理
	for (auto& object : objects) {
		object->Update();
	}

	
	for (Sprite* sprite : sprites) {
		// 平行移動用処理
		//Vector2 position = sprite->GetPosition();
		//position += Vector2{ 1.0f, 1.0f };
		//sprite->SetPosition(position);
		// 回転
		//float rotation = sprite->GetRotation();
		//rotation += 0.01f;
		//sprite->SetRotation(rotation);
		// 拡縮
		Vector2 size = sprite->GetSize();
		size.x = 50.0f;
		size.y = 50.0f;
		sprite->SetSize(size);
		// 色
		//Vector4 color = sprite->GetColor();
		//color.x += 0.01f;
		//if (color.x > 1.0f) {
		//	color.x -= 1.0f;
		//}
		//sprite->SetColor(color);
		//sprite->SetIsFlipX(true);
		//sprite->SetIsFlipY(true);
		sprite->Update();
	}


	// パーティクルのアップデート
	//ParticleManager::GetInstance()->Update();

}

void GameScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Object3dManager::GetInstance()->DrawSet();

	for (auto& object : objects) {
		object->Draw();
	}

	// スプライト描画前処理
	SpriteManager::GetInstance()->DrawSet();

	for (Sprite* sprite : sprites) {
		sprite->Draw();
	}

	// パーティクルの描画
//ParticleManager::GetInstance()->Draw();

}
