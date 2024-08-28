#pragma once
#include <map>
#include <memory>
#include <string>
#include "Model.h"
class ModelManager
{
private:
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = default;
	ModelManager& operator=(ModelManager&) = default;

public:
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	// 終了
	void Finalize();
	// 初期化
	void Initialize(DirectXManager* dxManager);

public:
	// モデルファイル読み込み
	void LoadModel(const std::string& filePath);
	// モデルの検索
	Model* FindModel(const std::string& filePath);
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

private:

	ModelLoader* modelLoader = nullptr;

public:
	ModelLoader* GetModelLoader() {return modelLoader;}
};

