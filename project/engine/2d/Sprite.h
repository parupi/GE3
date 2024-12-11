#pragma once
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "function.h"
class SpriteManager;
// スプライト
class Sprite {
public:
	~Sprite();
	void Initialize(std::string textureFilePath);
	void Update();
	void Draw();

private:

	void CreateVertexResource();
	void CreateIndexResource();
	void CreateMaterialResource();
	void CreateTransformationResource();
	void SetSpriteData();

	void AdjustTextureSize();
private:

	// 構造体
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material {
		Vector4 color;
		bool enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	SpriteManager* spriteManager_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	Material* materialData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	// テクスチャ番号
	uint32_t textureIndex = 0;
	std::string textureFilePath_;

	// 左右フリップ
	bool isFlipX_ = false;
	// 上下フリップ
	bool isFlipY_ = false;

private: // 実用化用変数
	EulerTransform transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	Vector2 position_ = { 0.0f, 0.0f };
	float rotation_ = 0.0f;
	Vector2 size_ = { 80.0f, 80.0f };

	Vector2 anchorPoint_ = { 0.0f, 0.0f };

	// テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f, 0.0f };
	// テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f, 100.0f };

public: // ゲッター//セッター //
	// 平行移動
	const Vector2& GetPosition() const { return position_; }
	void SetPosition(const Vector2& position) { position_ = position; }
	// 回転
	float GetRotation() const { return rotation_; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	// 拡縮
	const Vector2& GetSize() const { return size_; }
	void SetSize(const Vector2& size) { size_ = size; }
	// 色
	const Vector4& GetColor() const { return materialData_->color; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	// アンカー
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	// フリップX
	const bool& GetIsFlipX() const {return isFlipX_;}
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	// フリップY
	const bool& GetIsFlipY() const { return isFlipY_; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }
	// 左上座標
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }
	void SetTextureLeftTop(Vector2 textureLeftTop) { textureLeftTop_ = textureLeftTop; }
	// 切り出しサイズ
	const Vector2& GetTextureSize() const { return textureSize_; }
	void SetTextureSize(Vector2 textureSize) { textureSize_ = textureSize; }
}; 