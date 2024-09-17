#pragma once
#include "types.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

ID3D11Device* device = nullptr;
ID3D11DeviceContext* context = nullptr;
IDXGISwapChain* swapChain = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;
ID3D11Buffer* vertexBuffer = nullptr;
ID3D11InputLayout* inputLayout = nullptr;
ID3D11VertexShader* vertexShader = nullptr;
ID3D11PixelShader* pixelShader = nullptr;

const int WIDTH = 800;
const int HEIGHT = 600;

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

// Vertex Shader source
const char* vsSource = R"(
struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}
)";

// Pixel Shader source
const char* psSource = R"(
struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_Target {
    return input.color;
}
)";

void InitD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Width = WIDTH;
    scd.BufferDesc.Height = HEIGHT;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    // Create the device, device context, and swap chain
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &context);

    // Get the address of the back buffer and create a render target view
    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    backBuffer->Release();

    // Bind the render target view
    context->OMSetRenderTargets(1, &renderTargetView, NULL);

    // Set the viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(WIDTH);
    viewport.Height = static_cast<FLOAT>(HEIGHT);
    context->RSSetViewports(1, &viewport);

    // Initialize graphics and pipeline
    InitGraphics();
    InitPipeline();
}

// Clean up Direct3D
void CleanD3D() {
    swapChain->SetFullscreenState(FALSE, NULL);
    vertexBuffer->Release();
    inputLayout->Release();
    vertexShader->Release();
    pixelShader->Release();
    renderTargetView->Release();
    swapChain->Release();
    context->Release();
    device->Release();
}

// Initialize the geometry for rendering
void InitGraphics() {
    // Define vertices for a quad (2D rectangle)
    Vertex vertices[] = {
        { DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // Top-left
        { DirectX::XMFLOAT3( 0.5f,  0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Top-right
        { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }, // Bottom-left
        { DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) }  // Bottom-right
    };

    // Create vertex buffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
}

// Initialize the graphics pipeline (shaders, input layout)
void InitPipeline() {
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    // Compile and create the vertex shader
    D3DCompile(vsSource, strlen(vsSource), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &vsBlob, NULL);
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader);

    // Compile and create the pixel shader
    D3DCompile(psSource, strlen(psSource), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &psBlob, NULL);
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader);

    // Set the shaders
    context->VSSetShader(vertexShader, 0, 0);
    context->PSSetShader(pixelShader, 0, 0);

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    context->IASetInputLayout(inputLayout);

    vsBlob->Release();
    psBlob->Release();
}

// Render a frame
void RenderFrame() {
    // Clear the back buffer to a color
    float clearColor[] = { 0.1f, 0.2f, 0.3f, 1.0f };
    context->ClearRenderTargetView(renderTargetView, clearColor);

    // Select which vertex buffer to display
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set primitive topology
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Draw the rectangle
    context->Draw(4, 0);

    // Swap the back and front buffers
    swapChain->Present(0, 0);
}