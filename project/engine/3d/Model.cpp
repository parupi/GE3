#include "Model.h"
#include "function.h"
#include "TextureManager.h"
#include <Vector4.h>
#include <Vector2.h>
#include <ModelManager.h>
#include <WorldTransform.h>


void Model::Initialize(ModelLoader* modelManager, const std::string& directoryPath, const std::string& fileName)
{
	modelLoader_ = modelManager;

	// モデルの読み込み
	modelData_ = LoadModelFile(directoryPath, fileName);

	CreateVertexResource();

	// .objの参照しているテクスチャファイルの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);
}

void Model::Draw(WorldTransform& transform, Camera* camera)
{
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = transform.GetMatWorld() * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = transform.GetMatWorld();
	}
	transform.SetMapWVP(modelData_.rootNode.localMatrix * worldViewProjectionMatrix);
	transform.SetMapWorld(modelData_.rootNode.localMatrix * transform.GetMatWorld());

	// wvp用のCBufferの場所を設定
	modelLoader_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transform.GetConstBuffer()->GetGPUVirtualAddress());
	// VertexBufferViewを設定
	modelLoader_->GetDxManager()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// SRVのDescriptorTableの先頭を設定。
	modelLoader_->GetSrvManager()->SetGraphicsRootDescriptorTable(2, modelData_.material.textureIndex);
	// ドローコール
	modelLoader_->GetDxManager()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::CreateVertexResource()
{
	// 頂点リソースを作る
	vertexResource_ = modelLoader_->GetDxManager()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();	// リソースの先頭アドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ
	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_)); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

//Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
//{
//	MaterialData materialData;
//	std::string line;
//	std::ifstream file(directoryPath + "/" + filename);
//	assert(file.is_open());
//
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		if (identifier == "newmtl") {
//			s >> materialData.name;
//		}
//		else if (identifier == "Ns") {
//			s >> materialData.Ns;
//		}
//		else if (identifier == "Ka") {
//			s >> materialData.Ka.r >> materialData.Ka.g >> materialData.Ka.b;
//		}
//		else if (identifier == "Kd") {
//			s >> materialData.Kd.r >> materialData.Kd.g >> materialData.Kd.b;
//		}
//		else if (identifier == "Ks") {
//			s >> materialData.Ks.r >> materialData.Ks.g >> materialData.Ks.b;
//		}
//		else if (identifier == "Ni") {
//			s >> materialData.Ni;
//		}
//		else if (identifier == "d") {
//			s >> materialData.d;
//		}
//		else if (identifier == "illum") {
//			s >> materialData.illum;
//		}
//		else if (identifier == "map_Kd") {
//			std::string textureFilename;
//			s >> textureFilename;
//			materialData.textureFilePath = directoryPath + "/" + textureFilename;
//		}
//	}
//	return materialData;
//}

Model::ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;



	// モデルの生成とファイルの読み込み
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないメッシュは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応

		// Faceを解析する
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみをサポート

			// Vertexを解析する
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				Model::VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHandedは z*=-1 で、右手->左手に変換されるらしいので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}
	}

	// materialの解析を行う
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData.rootNode = ReadNode(scene->mRootNode);
	return modelData;
}

Model::Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトルを行ベクトルに転置
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	result.localMatrix.m[0][3] = aiLocalMatrix[0][3];

	result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	result.localMatrix.m[1][3] = aiLocalMatrix[1][3];

	result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[2][3];

	result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	result.name = node->mName.C_Str(); // node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読むことで階層構造を作る
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

void Model::SetVertices(VertexData vertex)
{
	modelData_.vertices.push_back(vertex);
}
