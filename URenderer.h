#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include "Utils.h"
#include "Types.h"
#include <assert.h>
#include "ULog.h"
#include "UCamera.h"

static float ROT = 0;

class URenderer
{
private:
    // URenderer를 주입하지 않도록 하기 위해 싱글턴으로 변경함.
    URenderer() {};
    URenderer(const URenderer& ref) {};
    URenderer& operator=(const URenderer& ref) {};
    ~URenderer() {};
    bool bIsInitialize = false;
public:
    static URenderer& GetInstance()
    {
        static URenderer instance;
        return instance;
    }

    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    IDXGISwapChain* SwapChain = nullptr;
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    ID3D11RasterizerState* RasterizerState = nullptr;
    ID3D11DepthStencilView* DepthStencilView = nullptr;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11PixelShader* PixelShader2 = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;
    ID3D11Buffer* ConstantBuffer = nullptr;
    ID3D11Buffer* ConstantBuffer2 = nullptr;
    ID3D11BlendState* blendState = nullptr;

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 블렌딩 팩터 (기본값)
    UINT sampleMask = 0xFFFFFFFF; // 샘플 마스크 (기본값)
  
    UCamera* MainCamera = nullptr;

    void SetMainCamera(UCamera* camera) {
        MainCamera = camera;
    }

    void Initialize(HWND hWnd)
    {
        // 장치 및 스왑 체인 생성
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = 1024;
        swapChainDesc.BufferDesc.Height = 1024;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = TRUE;

        D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION,
            &swapChainDesc, &SwapChain, &Device,
            nullptr, &DeviceContext
        );

        // 렌더 타겟 뷰 생성
        ID3D11Texture2D* backBuffer = nullptr;
        SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        Device->CreateRenderTargetView(backBuffer, nullptr, &RenderTargetView);
        backBuffer->Release();

        // 깊이 스텐실 버퍼 생성
        D3D11_TEXTURE2D_DESC depthStencilDesc = {};
        depthStencilDesc.Width = 1024;
        depthStencilDesc.Height = 1024;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* depthStencilBuffer = nullptr;
        Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
        Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &DepthStencilView);
        depthStencilBuffer->Release();

        // Blend
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = FALSE; // 알파 투 커버리지 비활성화
        blendDesc.IndependentBlendEnable = FALSE; // 독립 블렌딩 비활성화
        blendDesc.RenderTarget[0].BlendEnable = TRUE; // 블렌딩 활성화
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 소스 알파
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 1 - 소스 알파
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 블렌딩 연산: 더하기
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // 소스 알파
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // 목적지 알파
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // 알파 블렌딩 연산: 더하기
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 모든 채널 쓰기 활성화

        Device->CreateBlendState(&blendDesc, &blendState);

        // 래스터라이저 설정
        D3D11_RASTERIZER_DESC rasterizerdesc = {};
        rasterizerdesc.FillMode = D3D11_FILL_SOLID; 
        //rasterizerdesc.FillMode = D3D11_FILL_WIREFRAME;
        rasterizerdesc.CullMode = D3D11_CULL_BACK;
        rasterizerdesc.FrontCounterClockwise = FALSE;
        Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);

        // 뷰포트 설정
        D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 1024.0f, 1024.0f, 0.0f, 1.0f };
        DeviceContext->RSSetViewports(1, &viewport);
        DeviceContext->RSSetState(RasterizerState);

        // 셰이더 컴파일 및 생성
        #if defined(DEBUG) || defined(_DEBUG)

        ID3DBlob* vsBlob = nullptr;
        D3DCompileFromFile(L"ProjectionVertexShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vsBlob, nullptr);
        Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &VertexShader);

        ID3DBlob* psBlob = nullptr;
        D3DCompileFromFile(L"DefaultPixelShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psBlob, nullptr);
        Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &PixelShader);

        ID3DBlob* ps2Blob = nullptr;
        D3DCompileFromFile(L"SingleColorPixelShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &ps2Blob, nullptr);
        Device->CreatePixelShader(ps2Blob->GetBufferPointer(), ps2Blob->GetBufferSize(), nullptr, &PixelShader2);
        
        #else

        ID3DBlob* vsBlob = nullptr;
        D3DReadFileToBlob(L"ProjectionVertexShader.cso", &vsBlob);
        Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &VertexShader);

        ID3DBlob* psBlob = nullptr;
        D3DReadFileToBlob(L"DefaultPixelShader.cso", &psBlob);
        Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &PixelShader);

        ID3DBlob* ps2Blob = nullptr;
        D3DReadFileToBlob(L"SingleColorPixelShader.cso", &ps2Blob);
        Device->CreatePixelShader(ps2Blob->GetBufferPointer(), ps2Blob->GetBufferSize(), nullptr, &PixelShader2);

        #endif
        
        // 입력 레이아웃 생성
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        Device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &InputLayout);
        vsBlob->Release();
        psBlob->Release();
        ps2Blob->Release();

        // 상수 버퍼 생성
        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth = sizeof(FMatrix) * 3;
        constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Device->CreateBuffer(&constantBufferDesc, nullptr, &ConstantBuffer);

        D3D11_BUFFER_DESC constantBufferDesc2 = {};
        constantBufferDesc2.ByteWidth = sizeof(FVector4) * 3;
        constantBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
        constantBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Device->CreateBuffer(&constantBufferDesc2, nullptr, &ConstantBuffer2);
        
        bIsInitialize = true;
    }
    
    // 정점 버퍼 생성
    ID3D11Buffer* CreateVertexBuffer(FVertexSimple vertices[], uint32 numVertices) {
         D3D11_BUFFER_DESC vertexBufferDesc = {};
         vertexBufferDesc.ByteWidth = sizeof(FVertexSimple) * numVertices;
         vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
         vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

         ID3D11Buffer* VertexBuffer;
         D3D11_SUBRESOURCE_DATA vertexBufferData = { vertices };
         Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &VertexBuffer);
         return VertexBuffer;
    }

    // Overload for TArray data
    ID3D11Buffer* CreateVertexBuffer(TArray<FVertexSimple> vertices, uint32 numVertices) {
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(FVertexSimple) * numVertices;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        ID3D11Buffer* VertexBuffer;
        D3D11_SUBRESOURCE_DATA vertexBufferData = { vertices.data() };
        Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &VertexBuffer);
        return VertexBuffer;
    }

    // 인덱스 버퍼 생성
    ID3D11Buffer* CreateIndexBuffer(uint32 indices[], uint32 numIndices) {
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(uint32) * numIndices;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        ID3D11Buffer* IndexBuffer;
        D3D11_SUBRESOURCE_DATA indexBufferData = { indices };
        Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &IndexBuffer);
        return IndexBuffer;
    }

    // Overload for TArray data
    ID3D11Buffer* CreateIndexBuffer(TArray<uint32> indices, uint32 numIndices) {
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(uint32) * numIndices;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        ID3D11Buffer* IndexBuffer;
        D3D11_SUBRESOURCE_DATA indexBufferData = { indices.data() };
        Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &IndexBuffer);
        return IndexBuffer;
    }

    void Prepare()
    {
        // 화면 초기화
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        DeviceContext->ClearRenderTargetView(RenderTargetView, clearColor);
        DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // 렌더 타겟 및 깊이 스텐실 설정
        DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

        // 셰이더 및 입력 레이아웃 설정
        DeviceContext->VSSetShader(VertexShader, nullptr, 0);
        DeviceContext->IASetInputLayout(InputLayout);
    }

    void PreparePixelShader() {
        DeviceContext->PSSetShader(PixelShader, nullptr, 0);
    }

    void PreparePixelShader2() {
        DeviceContext->PSSetShader(PixelShader2, nullptr, 0);
    }

    void UpdateConstant(const FConstants& m_constantBufferData)
    {
        // 상수 버퍼 업데이트
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &m_constantBufferData, sizeof(FConstants));
        DeviceContext->Unmap(ConstantBuffer, 0);

        DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
    }

    void UpdateConstant2(const FVector4& Color)
    {
        // 상수 버퍼 업데이트
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        DeviceContext->Map(ConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &Color, sizeof(FVector4));
        DeviceContext->Unmap(ConstantBuffer2, 0);

        DeviceContext->PSSetConstantBuffers(1, 1, &ConstantBuffer2);
    }

    void RenderLine(ID3D11Buffer* VertexBuffer, ID3D11Buffer* IndexBuffer, uint32 numIndex) {
        // 정점 및 인덱스 버퍼 설정
        UINT stride = sizeof(FVertexSimple);
        UINT offset = 0;
        DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
        DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // 프리미티브 토폴로지 설정
        DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        // 그리기 호출
        DeviceContext->DrawIndexed(numIndex, 0, 0);
    }

    void RenderPrimitive(ID3D11Buffer* VertexBuffer, ID3D11Buffer* IndexBuffer, uint32 numIndex) {
        // 정점 및 인덱스 버퍼 설정
        UINT stride = sizeof(FVertexSimple);
        UINT offset = 0;
        DeviceContext->OMSetBlendState(blendState, blendFactor, sampleMask);
        DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
        DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // 프리미티브 토폴로지 설정
        DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // 그리기 호출
        DeviceContext->DrawIndexed(numIndex, 0, 0);
    }

    void SwapBuffer() {
        // 화면 출력
        SwapChain->Present(1, 0);
    }

    void Release()
    {
        if (DeviceContext) DeviceContext->ClearState();
        if (RenderTargetView) RenderTargetView->Release();
        if (RasterizerState) RasterizerState->Release();
        if (DepthStencilView) DepthStencilView->Release();
        if (SwapChain) SwapChain->Release();
        if (DeviceContext) DeviceContext->Release();
        if (Device) Device->Release();
    }
};