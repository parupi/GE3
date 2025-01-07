#pragma once
#include "Line3dManager.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <memory>
#include <d3d12.h>
#include <wrl.h>

class Line3d {
public:
    Line3d() = default;
    ~Line3d();

    void Initialize();
    void Update();
    void Draw(const Matrix4x4& worldViewProjectionMatrix);

private:
    void CreateVertexResource();
    void CreateConstantBuffer();

private:
    struct VertexPosColor {
        Vector3 pos;
        Vector4 color;
    };

    struct Mesh {
        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
        D3D12_VERTEX_BUFFER_VIEW vbView{};
        VertexPosColor* vertMap = nullptr;
    };

    // 線分の最大数
    static const int32_t kMaxLineCount = 4096;

    Line3dManager* lineManager_ = Line3dManager::GetInstance();
    std::unique_ptr<Mesh> mesh_;
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_;
    Matrix4x4* constantBufferData_ = nullptr;

    // 色やその他の状態を保持するためのプロパティ
    Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
};
