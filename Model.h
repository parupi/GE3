#pragma once
#include "ModelManager.h"
#include "math/Vector4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "math/Matrix4x4.h"
#include <fstream>
class Model
{
public: // メンバ関数
	// 初期化
	void Initialize(ModelManager* modelManager);

	void Draw();
private:
	void CreateVertexResource();
	void CreateMaterialResource();

private: // 構造体

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
	ModelManager* modelManager_ = nullptr;

	ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	
public:
	// mtlファイルを読む関数
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// OBJファイルを読む関数
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);


};

