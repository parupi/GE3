#pragma once
#include "Object3dManager.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "math/function.h"
#include <fstream>
class Object3dManager;

class Object3d
{
public: // メンバ関数
	void Initialize(Object3dManager* objectManager);
	void Update();
	void Draw();
private:

	void CreateVertexResource();
	void CreateMaterialResource();
	void CreateWVPResource();
	void CreateDirectionalLightResource();

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

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight {
		Vector4 color;		//!< ライトの色
		Vector3 direction;	//!< ライトの向き
		float intensity;	//!< 輝度
	};

private: // メンバ変数
	ModelData modelData_;

	Object3dManager* objectManager_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;
	TransformationMatrix* wvpData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;


	Transform transform_;
	Transform cameraTransform_;
public:
	// mtlファイルを読む関数
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// OBJファイルを読む関数
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);


};

