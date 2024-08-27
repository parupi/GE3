#pragma once
#include "SpriteManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "math/function.h"
class SpriteManager;
// スプライト
class Sprite {
public:
	void Initialize(SpriteManager* spriteManager);
	void Update();
	void Draw();

private:

	void CreateVertexResource();
	void CreateIndexResource();
	void CreateMaterialResource();
	void CreateTransformationResource();
	void SetSpriteData();

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

private: // 実用化用変数
	Transform transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	Vector2 position_ = { 0.0f, 0.0f };
	float rotation_ = 0.0f;
	Vector2 size_ = {160.0f, 90.0f };

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
};