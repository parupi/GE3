#include "ParticleManager.h"
#include <function.h>
#include <TextureManager.h>
#include <numbers>
#include <imgui.h>

ParticleManager* ParticleManager::instance = nullptr;
std::random_device seedGenerator;
std::mt19937 randomEngine(seedGenerator());

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager();
	}
	return instance;
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::Initialize(DirectXManager* dxManager, SrvManager* srvManager)
{
	dxManager_ = dxManager;
	srvManager_ = srvManager;
	// ランダムエンジンの初期化

	// PSO関連
	CreateRootSignature();
	inputLayoutDesc_ = CreateInputElementDesc();
	CreateBlendState();
	CreateRasterizerState();
	LoadShader();
	CreatePipelineState();
	// リソースの生成と値の設定
	CreateParticleResource();
	CreateMaterialResource();
}


void ParticleManager::Update()
{
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, camera_->GetRotate(), camera_->GetTranslate());
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
	Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;

	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = backToFrontMatrix * cameraMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (auto& [groupName, particleGroup] : particleGroups_) {


		uint32_t numInstance = 0;

		for (auto particleIterator = particleGroup.particleList.begin(); particleIterator != particleGroup.particleList.end();) {
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				// パーティクルが生存時間を超えたら削除
				particleIterator = particleGroup.particleList.erase(particleIterator);
				continue;
			}

			if (numInstance >= kNumMaxInstance) {
				break;
			}

			// パーティクルの更新処理
			(*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
			(*particleIterator).currentTime += kDeltaTime;
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

			// ワールド行列の計算
			scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
			translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);
			Matrix4x4 worldMatrix{};
			if (isBillboard) {
				worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
			}
			else {
				worldMatrix = MakeAffineMatrix((*particleIterator).transform.scale, (*particleIterator).transform.rotate, (*particleIterator).transform.translate);
			}
			Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

			// インスタンシングデータの設定
			if (numInstance < kNumMaxInstance) {
				instancingData_[numInstance].WVP = worldViewProjectionMatrix;
				instancingData_[numInstance].World = worldMatrix;
				instancingData_[numInstance].color = (*particleIterator).color;
				instancingData_[numInstance].color.w = alpha;
				++numInstance;
			}

			++particleIterator;
		}

		// インスタンス数の更新
		particleGroup.instanceCount = numInstance;

		// GPU メモリにインスタンスデータを書き込む
		if (particleGroup.instancingDataPtr) {
			std::memcpy(particleGroup.instancingDataPtr, instancingData_, sizeof(ParticleForGPU) * numInstance);
		}
	}
}

void ParticleManager::Draw()
{
	// グラフィックスパイプラインの設定
	auto commandList = dxManager_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// すべてのパーティクルグループを描画
	for (auto& [groupName, particleGroup] : particleGroups_) {
		if (particleGroup.instanceCount > 0) {
			// マテリアルCBufferの場所を指定
			commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
			// SRVのDescriptorTableを設定
			srvManager_->SetGraphicsRootDescriptorTable(1, particleGroup.srvIndex);
			// テクスチャのSRVのDescriptorTableを設定
			   // テクスチャのSRVのDescriptorTableを設定
			srvManager_->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureIndexByFilePath(particleGroup.materialData.textureFilePath));
			
			commandList->DrawInstanced(6, particleGroup.instanceCount, 0, 0);
		}
	}
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
	if (particleGroups_.contains(name)) {
		// 登録済みの名前なら早期リターン
		return;
	}

	// グループを追加
	particleGroups_[name] = ParticleGroup();
	ParticleGroup& particleGroup = particleGroups_[name];

	// テクスチャを読み込む（未読み込みならロードする）
	particleGroup.materialData.textureFilePath = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(particleGroup.materialData.textureFilePath);
	particleGroup.materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	// インスタンシング用リソースの生成
	particleGroup.instancingResource = dxManager_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	// インスタンシング用にSRVを確保してSRVインデックスを記録
	//particleGroup.srvIndex = srvManager_->Allocate();

	// インスタンシングデータを書き込むためのポインタを取得
	particleGroup.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instancingDataPtr));

	// SRVを作成するDescriptorの場所を決める
	particleGroup.srvIndex = srvManager_->Allocate() + 1;

	// SRVの生成
	srvManager_->CreateSRVforStructuredBuffer(particleGroup.srvIndex, particleGroup.instancingResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));

	// インスタンス数を初期化
	particleGroup.instanceCount = 0;
}

void ParticleManager::CreateRootSignature()
{
	// Particle用のRootSignature
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;		// 0から始まる
	descriptorRangeForInstancing[0].NumDescriptors = 1;			// 数は1つ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	// SRVを使う
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;														// 0から始まる
	descriptorRange[0].NumDescriptors = 1;															// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。PixelShaderのMaterialとVertezShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;			// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;							// レジスタ番号0とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// VertexShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;			// Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);		// Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;			// DescriptorTableで使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;						// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;					// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);		// Tableで利用する数								// レジスタ番号1を使う

	descriptionRootSignature.pParameters = rootParameters;						// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);			// 配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリをもとに生成

	hr = dxManager_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

D3D12_INPUT_LAYOUT_DESC ParticleManager::CreateInputElementDesc()
{
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	return inputLayoutDesc;
}

void ParticleManager::CreateBlendState()
{
	// すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].BlendEnable = TRUE;
	blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

void ParticleManager::CreateRasterizerState()
{
	// 裏面(時計回り)を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void ParticleManager::LoadShader()
{
	// Shaderをコンパイルする
	vertexShaderBlob_ = dxManager_->CompileShader(L"./resource/shaders/Particle.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = dxManager_->CompileShader(L"./resource/shaders/Particle.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}

void ParticleManager::CreatePipelineState()
{
	HRESULT hr;

	// PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();			// RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;			// InputLayout
	//graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;			// InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize() };			// vertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };			// PixelShade
	graphicsPipelineStateDesc.BlendState = blendDesc_;					// BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;			// RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	hr = dxManager_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void ParticleManager::CreateParticleResource()
{
	// インスタンス用のTransformationMatrixリソースを作る
	instancingResource_ = dxManager_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	// 書き込むためのアドレスを取得
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {

		instancingData_[index].WVP = MakeIdentity4x4();

		instancingData_[index].World = MakeIdentity4x4();
		instancingData_[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	// モデルの読み込み
	ModelData modelData;
	modelData.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	//modelData.material.textureFilePath = "resource/uvChecker.png";
	// 頂点リソースを作る
	vertexResource = dxManager_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource->GetGPUVirtualAddress();	// リソースの先頭アドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());		// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);	// 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));		// 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void ParticleManager::CreateMaterialResource()
{
	// マテリアル用のリソースを作る。
	materialResource_ = dxManager_->CreateBufferResource(sizeof(Material));
	// マテリアルにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 白を入れる
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
}

ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	Particle particle;
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 randomTranslate = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.transform.translate = translate + randomTranslate;
	particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.color = { distColor(randomEngine) , distColor(randomEngine) , distColor(randomEngine) , 1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0.0f;

	return particle;
}

std::list<ParticleManager::Particle> ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
	ParticleGroup& particleGroup = particleGroups_[name];
	std::list<Particle> newParticles;
	for (uint32_t nowCount = 0; nowCount < count; ++nowCount) {
		Particle particle =MakeNewParticle(randomEngine, position);
		newParticles.push_back(particle);
	}
	particleGroup.particleList.splice(particleGroup.particleList.end(), newParticles);
	return newParticles;
}