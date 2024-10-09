#include "ModelManager.h"

std::unique_ptr<ModelManager> ModelManager::instance = nullptr;
std::once_flag ModelManager::initInstanceFlag;

ModelManager* ModelManager::GetInstance()
{
	std::call_once(initInstanceFlag, []() {
		instance = std::make_unique<ModelManager>();
		});
	return instance.get();
}

void ModelManager::Initialize(DirectXManager* dxManager)
{
	modelLoader = std::make_unique<ModelLoader>();
	modelLoader->Initialize(dxManager);
}

void ModelManager::LoadModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models.contains(filePath)) {
		// 読み込み済みなら早期return
		return;
	}
	// モデルの生成とファイルの読み込み
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelLoader.get(), "resource", filePath);

	// モデルをmapコンテナに格納する
	models.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}

	// ファイル名が一致しなければnull
	return nullptr;
}
