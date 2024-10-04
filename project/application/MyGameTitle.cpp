#include "MyGameTitle.h"

void MyGameTitle::Initialize()
{
	GuchisFramework::Initialize();
	// ImGui初期化
	ImGuiManager::GetInstance()->Initialize(winManager, directXManager);
	// 2Dテクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(directXManager, srvManager);
	// 3Dテクスチャマネージャーの初期化
	ModelManager::GetInstance()->Initialize(directXManager);
	// パーティクルマネージャーの初期化
	//ParticleManager::GetInstance()->Initialize(directXManager, srvManager);

	// スプライト共通部の初期化
	spriteManager = new SpriteManager();
	spriteManager->Initialize(directXManager);
	// オブジェクト共通部
	objectManager = new Object3dManager();
	objectManager->Initialize(directXManager);
	// カメラの生成
	camera = new Camera();
	//camera->SetRotate({})
	//camera->SetTranslate({})
	objectManager->SetDefaultCamera(camera);

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

	// Spriteの初期化

	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		if (i == 1) {
			sprite->Initialize(spriteManager, "resource/monsterBall.png");
		}
		else {
			sprite->Initialize(spriteManager, "resource/uvChecker.png");
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
		object->Initialize(objectManager);
		object->SetModel(model);
		objects.push_back(object);
	}

	// それぞれのObject3dに位置や回転を設定
	models[0]->SetPosition({ -2.0f, 0.0f, 0.0f });
	models[0]->SetRotation({ 0.0f, 0.0f, 0.0f });

	models[1]->SetPosition({ 2.0f, 0.0f, 0.0f });
	models[1]->SetRotation({ 0.0f, 45.0f, 0.0f });


}

void MyGameTitle::Finalize()
{

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	//ParticleManager::GetInstance()->Finalize();
	delete spriteManager;
	spriteManager = nullptr;
	delete objectManager;
	objectManager = nullptr;

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
	ImGuiManager::GetInstance()->Finalize();
	GuchisFramework::Finalize();
}

void MyGameTitle::Update()
{
	GuchisFramework::Update();
	ImGuiManager::GetInstance()->Begin();

	// ゲームの処理
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

	camera->Update();
	for (auto& object : objects) {
		object->Update();
	}
	// パーティクルのアップデート
	//ParticleManager::GetInstance()->Update();

	ImGuiManager::GetInstance()->End();
}

void MyGameTitle::Draw()
{
	directXManager->BeginDraw();
	srvManager->BeginDraw();

	objectManager->DrawSet();

	for (auto& object : objects) {
		object->Draw();
	}


	spriteManager->DrawSet();

	for (Sprite* sprite : sprites) {
		sprite->Draw();
	}
	// パーティクルの描画
	//ParticleManager::GetInstance()->Draw();

	ImGuiManager::GetInstance()->Draw();

	directXManager->EndDraw();
}

