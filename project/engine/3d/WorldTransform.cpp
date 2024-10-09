#include "WorldTransform.h"
#include <function.h>
#include <DirectXManager.h>

void WorldTransform::Initialize()
{
	// ワールド行列の初期化
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 定数バッファ生成
	CreateConstBuffer();

	// 定数バッファへ初期行列を転送
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	// 定数バッファの設定
	HRESULT hr;

    // リソースのサイズを定義（定数バッファは256バイトアラインメント）
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD; // CPUからアクセス可能なアップロードヒープ
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = (sizeof(ConstBufferDataWorldTransform) + 255) & ~255; // 256バイトアラインメント
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 定数バッファリソースを作成
    hr = DirectXManager::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffer_)
    );
    if (FAILED(hr)) {
        // エラーハンドリング
        throw std::runtime_error("Failed to create constant buffer.");
    }

}

void WorldTransform::Map()
{
    // 定数バッファをマップ（CPUがアクセスできるようにする）
    HRESULT hr = constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
    if (FAILED(hr)) {
        // エラーハンドリング
        throw std::runtime_error("Failed to map constant buffer.");
    }
}

void WorldTransform::TransferMatrix()
{
    // ワールド行列を定数バッファに転送
    if (constMap != nullptr) {
        // 親が存在する場合、親のワールド行列を掛け合わせる
        if (parent_) {
            Matrix4x4 parentMatrix = parent_->matWorld_;
            matWorld_ = matWorld_ * parentMatrix; // 親の行列と自身の行列を合成
        }

        constMap->matWorld = matWorld_; // 定数バッファに行列をコピー
    }
}
