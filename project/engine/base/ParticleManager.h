#pragma once
#pragma once
#include <DirectXManager.h>
#include <SrvManager.h>
#include <random>
#include <Vector4.h>
#include <Matrix4x4.h>
#include <Camera.h>
#include <Vector2.h>
class ParticleManager
{
private:
	static ParticleManager* instance;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;
	ParticleManager& operator=(ParticleManager&) = default;


public:
	// シングルトンインスタンスの取得
	static ParticleManager* GetInstance();
	// 終了
	void Finalize();
	// 初期化
	void Initialize(DirectXManager* dxManager, SrvManager* srvManager);
	// 更新
	void Update();
	// 描画
	void Draw();
	// パーティクルグループを登録する
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	// カメラをセットする
	void SetCamera(Camera* camera) { camera_ = camera; }

private: // 構造体

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

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	struct ParticleGroup {
		MaterialData materialData;  // マテリアルデータ
		std::list<Particle> particleList;  // パーティクルのリスト
		uint32_t srvIndex;  // インスタンシング用SRVインデックス
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;  // インスタンシングリソース
		uint32_t instanceCount;  // インスタンス数
		ParticleForGPU* instancingDataPtr;  // インスタンシングデータを書き込むためのポインタ
	};

	//struct Emitter {
	//	Transform transform; //!< エミッタのTransform
	//	uint32_t count; //!< 発生数
	//	float frequency; //!< 発生頻度
	//	float frequencyTime; //!< 頻度用時刻
	//};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
		float padding[3];
	};
private:

	void CreateRootSignature();
	D3D12_INPUT_LAYOUT_DESC CreateInputElementDesc();
	void CreateBlendState();
	void CreateRasterizerState();
	void LoadShader();
	void CreatePipelineState();
	// パーティクル用のリソースの生成
	void CreateParticleResource();
	// WVP用のリソースを生成 
	void CreateMaterialResource();
	// パーティクルを生成する関数
	Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);

public:

	// nameで指定した名前のパーティクルグループにパーティクルを発生させる関数
	std::list<Particle> Emit(const std::string name, const Vector3& position, uint32_t count);

private:
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	// BlendState
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	DirectXManager* dxManager_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	Camera* camera_ = nullptr;
	// ランダム用変数宣言
	std::mt19937 randomEngine;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	const uint32_t kNumMaxInstance = 128;	// 最大インスタンス数
	// パーティクル用リソースの宣言
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	ParticleForGPU* instancingData_ = nullptr;
	Material* materialData_;
	VertexData* vertexData_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_;

	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	bool isBillboard = true;
	const float kDeltaTime = 1.0f / 60.0f;

	Matrix4x4 scaleMatrix;
	Matrix4x4 translateMatrix;


	std::list<Particle> particles;
	uint32_t numInstance = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
};
