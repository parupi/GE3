#pragma once
#include "ModelLoader.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include <fstream>
class Model
{
public: // メンバ関数
	// 初期化
	void Initialize(ModelLoader* modelLoader, const std::string& directoryPath, const std::string& fileName);

	void Draw();
private:
	void CreateVertexResource();
private: // 構造体
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Color {
		float r, g, b;
	};

	struct MaterialData {
		std::string name;
		float Ns;
		Color Ka;	// 環境光色
		Color Kd;	// 拡散反射色
		Color Ks;	// 鏡面反射光
		float Ni;
		float d;
		uint32_t illum;
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

private:
	ModelLoader* modelLoader_ = nullptr;

	ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;


	VertexData* vertexData_ = nullptr;


	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

private: // 実用化用変数
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };
	Vector3 size_ = { 1.0f, 1.0f, 1.0f};
public:
	// mtlファイルを読む関数
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// OBJファイルを読む関数
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
public: // ゲッター // セッター //

};

