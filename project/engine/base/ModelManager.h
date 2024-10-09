#pragma once
#include <map>
#include <memory>
#include <string>
#include "Model.h"
#include <mutex>
class ModelManager
{
private:
	static std::unique_ptr<ModelManager> instance;
	static std::once_flag initInstanceFlag;

	ModelManager(ModelManager&) = default;
	ModelManager& operator=(ModelManager&) = default;
public:
	ModelManager() = default;
	~ModelManager() = default;
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();
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

	std::unique_ptr<ModelLoader> modelLoader = nullptr;

public:
	ModelLoader* GetModelLoader() {return modelLoader.get();}
};

