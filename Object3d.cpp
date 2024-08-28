#include "Object3d.h"
#include "Object3dManager.h"

#include "TextureManager.h"
#include "externals/imgui/imgui.h"

void Object3d::Initialize(Object3dManager* objectManager)
{
	objectManager_ = objectManager;

	CreateWVPResource();
	CreateDirectionalLightResource();

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WindowManager::kClientWidth) / float(WindowManager::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	wvpData_->World = worldViewProjectionMatrix;
	wvpData_->WVP = worldViewProjectionMatrix;
}

void Object3d::Draw()
{
	// wvp用のCBufferの場所を設定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	// directionalLightの場所を指定
	objectManager_->GetDxManager()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model_){
		model_->Draw();
	}
}

void Object3d::CreateWVPResource()
{
	// MVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	// 単位行列を書き込んでおく
	wvpData_->World = MakeIdentity4x4();
	wvpData_->WVP = MakeIdentity4x4();
}

void Object3d::CreateDirectionalLightResource()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = objectManager_->GetDxManager()->CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// 今回は白を書き込んで置く
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

//Object3d::MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
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
//
//Object3d::ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
//{
//	ModelData modelData;
//	std::vector<Vector4> positions;
//	std::vector<Vector3> normals;
//	std::vector<Vector2> texcoords;
//	std::string line;
//
//	std::ifstream file(directoryPath + "/" + filename);
//	assert(file.is_open());
//
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		if (identifier == "v") {
//			Vector4 position;
//			s >> position.x >> position.y >> position.z;
//			position.x *= -1.0f;
//			position.w = 1.0f;
//			positions.push_back(position);
//		}
//		else if (identifier == "vt") {
//			Vector2 texcoord;
//			s >> texcoord.x >> texcoord.y;
//			texcoord.y = 1.0f - texcoord.y;
//			texcoords.push_back(texcoord);
//		}
//		else if (identifier == "vn") {
//			Vector3 normal;
//			s >> normal.x >> normal.y >> normal.z;
//			normal.x *= -1.0f;
//			normals.push_back(normal);
//		}
//		else if (identifier == "f") {
//			VertexData triangle[3];
//			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
//				std::string vertexDefinition;
//				s >> vertexDefinition;
//				std::istringstream v(vertexDefinition);
//				uint32_t elementIndices[3];
//				for (int32_t element = 0; element < 3; ++element) {
//					std::string index;
//					std::getline(v, index, '/');
//					elementIndices[element] = std::stoi(index);
//				}
//				Vector4 position = positions[elementIndices[0] - 1];
//				Vector2 texcoord = texcoords[elementIndices[1] - 1];
//				Vector3 normal = normals[elementIndices[2] - 1];
//				VertexData vertex = { position, texcoord, normal };
//				triangle[faceVertex] = vertex;
//			}
//			modelData.vertices.push_back(triangle[2]);
//			modelData.vertices.push_back(triangle[1]);
//			modelData.vertices.push_back(triangle[0]);
//		}
//		else if (identifier == "mtllib") {
//			std::string materialFilename;
//			s >> materialFilename;
//			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
//		}
//	}
//
//	return modelData;
//}
