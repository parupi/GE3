#include "Line3d.h"
#include <cassert>
#include <d3dx12.h>

Line3d::~Line3d()
{
    if (mesh_ && mesh_->vertBuff) {
        mesh_->vertBuff->Unmap(0, nullptr);
    }
    if (constantBuffer_) {
        constantBuffer_->Unmap(0, nullptr);
    }
}

void Line3d::Initialize()
{
    CreateVertexResource();
    CreateConstantBuffer();
}

void Line3d::Update() {
    mesh_->vertMap[0].pos = { 0.0f, 0.0f, 0.0f };
    mesh_->vertMap[0].color = color_;
    mesh_->vertMap[1].pos = { 1.0f, 1.0f, 0.0f };
    mesh_->vertMap[1].color = color_;
}

void Line3d::Draw(const Matrix4x4& worldViewProjectionMatrix) {
    *constantBufferData_ = worldViewProjectionMatrix;

    auto commandList = lineManager_->GetDxManager()->GetCommandList();
    commandList->SetGraphicsRootConstantBufferView(0, constantBuffer_->GetGPUVirtualAddress());
    //commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    commandList->IASetVertexBuffers(0, 1, &mesh_->vbView);

    commandList->DrawInstanced(2, 1, 0, 0);
}

void Line3d::CreateVertexResource() {
    mesh_ = std::make_unique<Mesh>();

    const UINT bufferSize = sizeof(VertexPosColor) * kMaxLineCount;

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    HRESULT hr = lineManager_->GetDxManager()->GetDevice()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mesh_->vertBuff));
    assert(SUCCEEDED(hr));

    mesh_->vbView.BufferLocation = mesh_->vertBuff->GetGPUVirtualAddress();
    mesh_->vbView.SizeInBytes = bufferSize;
    mesh_->vbView.StrideInBytes = sizeof(VertexPosColor);

    hr = mesh_->vertBuff->Map(0, nullptr, reinterpret_cast<void**>(&mesh_->vertMap));
    assert(SUCCEEDED(hr));
}


void Line3d::CreateConstantBuffer() {
    constantBuffer_ = lineManager_->GetDxManager()->CreateBufferResource(sizeof(Matrix4x4));
    HRESULT hr = constantBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constantBufferData_));
    assert(SUCCEEDED(hr));
}

