#include "ParticleManager.h"
#include <function.h>
#include <TextureManager.h>
#include <numbers>

ParticleManager* ParticleManager::instance = nullptr;

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
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	// PSO関連
	CreateRootSignature();
	inputLayoutDesc_ = CreateInputElementDesc();
	CreateBlendState();
	CreateRasterizerState();
	LoadShader();
	CreatePipelineState();
	// リソースの生成と値の設定
	CreateParticleResource();
}

void ParticleManager::Update()
{
	for (auto& [groupName, particleGroup] : particleGroups_) {
		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera_->GetWorldMatrix());

		Matrix4x4 viewMatrix = Inverse(camera_->GetWorldMatrix());
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, camera_->GetProjectionMatrix());

		emitter.frequencyTime += kDeltaTime;
		if (emitter.frequencyTime >= emitter.frequency) {
			// パーティクルを生成して指定したパーティクルグループに追加
			Emit(groupName, emitter.transform.translate, 3);

			// 発生したパーティクルの時間をリセット
			emitter.frequencyTime -= emitter.frequency;
		}

		uint32_t numInstance = 0;  // 各グループごとにリセット

		for (auto particleIterator = particleGroup.particleList.begin(); particleIterator != particleGroup.particleList.end();) {
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = particleGroup.particleList.erase(particleIterator);
				continue;
			}

			(*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
			(*particleIterator).currentTime += kDeltaTime;
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

			Matrix4x4 scaleMatrix = ScaleMatrixFromVector3((*particleIterator).transform.scale);
			Matrix4x4 translateMatrix = TranslationMatrixFromVector3((*particleIterator).transform.translate);
			Matrix4x4 worldMatrix{};
			if (isBillboard) {
				worldMatrix = Multiply(Multiply(scaleMatrix, billboardMatrix), translateMatrix);
			}
			else {
				worldMatrix = MakeAffineMatrix((*particleIterator).transform.scale, (*particleIterator).transform.rotate, (*particleIterator).transform.translate);
			}

			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

			if (numInstance < kNumMaxInstance) {
				instancingData_[numInstance].WVP = worldViewProjectionMatrix;
				instancingData_[numInstance].World = worldMatrix;
				instancingData_[numInstance].color = (*particleIterator).color;
				instancingData_[numInstance].color.w = alpha;
				++numInstance;
			}

			++particleIterator;
		}

		// インスタンス数を更新
		particleGroup.instanceCount = numInstance;

		// 死んだパーティクルを削除する処理をループの外で行う
		particleGroup.particleList.remove_if([](const Particle& p) {
			return p.currentTime >= p.lifeTime;
			});
	}
}

void ParticleManager::Draw()
{
	// グラフィックスパイプラインの設定
	auto commandList = dxManager_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// すべてのパーティクルグループを描画
	for (auto& [groupName, particleGroup] : particleGroups_) {
		// テクスチャのSRVのDescriptorTableを設定
		commandList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetSrvHandleGPU(particleGroup.materialData.textureFilePath));

		// インスタンシングデータのSRVのDescriptorTableを設定
		// インスタンシングデータは通常定数バッファではなく、構造化バッファ
		//commandList->SetGraphicsRootShaderResourceView(1, particleGroup.instancingResource->GetGPUVirtualAddress());

		// パーティクルグループごとのインスタンス数を使って描画
		commandList->DrawInstanced(6, particleGroup.instanceCount, 0, 0);
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
	particleGroup.materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

	// インスタンシング用リソースの生成
	particleGroup.instancingResource = dxManager_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	// インスタンシング用にSRVを確保してSRVインデックスを記録
	particleGroup.srvIndex = srvManager_->Allocate();

	// インスタンシングデータを書き込むためのポインタを取得
	particleGroup.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instancingDataPtr));

	// SRV生成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // ParticleForGPU のフォーマットに応じて適切に設定
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = kNumMaxInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	auto srvHandleCPU = srvManager_->GetCPUDescriptorHandle(particleGroup.srvIndex);
	//dxManager_->GetDevice()->CreateShaderResourceView(particleGroup.instancingResource.Get(), &srvDesc, srvHandleCPU);

	// 必要に応じてUnmapを行う
	particleGroup.instancingResource->Unmap(0, nullptr);

	// インスタンス数を初期化
	//particleGroup.instanceCount = 0;

}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
	std::list<Particle> particles;
	for (uint32_t nowCount = 0; nowCount < count; ++nowCount) {
		particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translate));
	}
}


void ParticleManager::CreateRootSignature()
{

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。PixelShaderのMaterialとVertezShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;			// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;							// レジスタ番号0とバインド

	// Particle用のRootSignature
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;		// 0から始まる
	descriptorRangeForInstancing[0].NumDescriptors = 1;			// 数は1つ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	// SRVを使う
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// VertexShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;			// Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);		// Tableで利用する数

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;														// 0から始まる
	descriptorRange[0].NumDescriptors = 1;															// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;			// DescriptorTableで使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;						// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;					// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);		// Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;						// DescriptorTableで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;						// PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;										// レジスタ番号1を使う

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
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
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
	instancingResource_ = dxManager_->CreateBufferResource(sizeof(ParticleForGPU)* kNumMaxInstance);
	// 書き込むためのアドレスを取得
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
	}
	// 頂点バッファビューの作成
	vertexBufferView_.BufferLocation = instancingResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(ParticleForGPU) * kNumMaxInstance;
	vertexBufferView_.StrideInBytes = sizeof(ParticleForGPU);
}

ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	Particle particle;
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 randomTranslate = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.transform.translate = translate + randomTranslate;
	particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.color = { distribution(randomEngine) , distribution(randomEngine) , distribution(randomEngine) , 1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0.0f;

	return particle;
}
