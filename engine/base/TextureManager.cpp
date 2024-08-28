#include "TextureManager.h"


TextureManager* TextureManager::instance = nullptr;

// ImGuiで0番を使うため、1番から使用
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize(DirectXManager* dxManager)
{
	// SRVの数と同数
	textureData_.reserve(DirectXManager::kMaxSRVCount);

	dxManager_ = dxManager;
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	// 読み込み済みのテクスチャを検索
	auto it = std::find_if(
		textureData_.begin(),
		textureData_.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureData_.end()) {
		// 読み込み済みなら早期return
		return;
	}

	// テクスチャ枚数上限チェック
	assert(textureData_.size() + kSRVIndexTop < DirectXManager::kMaxSRVCount);

	// テクスチャファイルを呼んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// テクスチャデータを追加
	textureData_.resize(textureData_.size() + 1);
	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureData_.back();

	textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxManager_->CreateTextureResource(textureData.metadata);
	dxManager_->UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureData_.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxManager_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = dxManager_->GetSRVGPUDescriptorHandle(srvIndex);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureData.metadata.format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(textureData.metadata.mipLevels);
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	// 設定をもとにSRVの生成
	dxManager_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	auto it = std::find_if(
		textureData_.begin(),
		textureData_.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureData_.end()) {
		// 読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureData_.begin(), it));
		return textureIndex;
	}

	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex)
{
	assert(textureIndex < textureData_.size());

	TextureData& textureData = textureData_.at(textureIndex);
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(uint32_t textureIndex)
{
	// 範囲外指定違反チェック
	assert(textureIndex < textureData_.size());

	TextureData& textureData = textureData_.at(textureIndex);
	return textureData.metadata;
}
