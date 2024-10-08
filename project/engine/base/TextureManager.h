#pragma once
#include "DirectXManager.h"
#include "DirectXTex.h"
#include <d3d12.h>
#include <SrvManager.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
class TextureManager
{
private:
	static std::unique_ptr<TextureManager> instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;
public:
	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	// 終了
	void Finalize();
	// 初期化
	void Initialize(DirectXManager* dxManager, SrvManager* srvManager);

public:
	void LoadTexture(const std::string& filePath);

	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);
private:
	// テクスチャ1枚分のデータ
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	// テクスチャデータ
	//std::vector<TextureData> textureData_;
	std::unordered_map<std::string, TextureData> textureData_;

	DirectXManager* dxManager_ = nullptr;
	SrvManager* srvManager_ = nullptr;
public:
	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;

};