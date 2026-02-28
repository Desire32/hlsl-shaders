#include "Scene.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"
#include "State.h"
#include "Shader.h"
#include "Input.h"
#include "Common.h"
#include "CVector2.h"
#include "CVector3.h"
#include "CMatrix4x4.h"
#include "MathHelpers.h"
#include "GraphicsHelpers.h"
#include "ColourRGBA.h"
#include <sstream>
#include <memory>

const float ROTATION_SPEED = 2.0f;
const float MOVEMENT_SPEED = 50.0f;

Mesh* gCubeMesh = nullptr;
Mesh* gFloorMesh = nullptr;
Mesh* gLightMesh = nullptr;
Mesh* gTeaPotMesh = nullptr;

Model* gCube = nullptr;
Model* gCube2 = nullptr;
Model* gFloor = nullptr;
Model* gTeaPot = nullptr;

Camera* gCamera = nullptr;

// Light
const int NUM_LIGHTS = 1;
struct Light
{
    Model* model;
    CVector3 colour;
    float    strength;
};
Light gLights[NUM_LIGHTS];

CVector3   gAmbientColour = { 0.2f, 0.2f, 0.3f };
float      gSpecularPower = 256.0f;
ColourRGBA gBackgroundColor = { 0.0f, 0.125f, 0.3f, 1.0f };

const float gLightOrbit = 20.0f;
const float gLightOrbitSpeed = 0.7f;
bool lockFPS = true;

//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
PerFrameConstants gPerFrameConstants;
ID3D11Buffer* gPerFrameConstantBuffer;
PerModelConstants gPerModelConstants;
ID3D11Buffer* gPerModelConstantBuffer;

//--------------------------------------------------------------------------------------
// Textures
//--------------------------------------------------------------------------------------
ID3D11Resource* gCubeDiffuseMap = nullptr;
ID3D11ShaderResourceView* gCubeDiffuseMapSRV = nullptr;

ID3D11Resource* gFloorDiffuseMap = nullptr;
ID3D11ShaderResourceView* gFloorDiffuseMapSRV = nullptr;

ID3D11Resource* gTeaPotDiffuseMap = nullptr;
ID3D11ShaderResourceView* gTeaPotDiffuseMapSRV = nullptr;

ID3D11Resource* gLightDiffuseMap = nullptr;
ID3D11ShaderResourceView* gLightDiffuseMapSRV = nullptr;

//--------------------------------------------------------------------------------------
// InitGeometry
//--------------------------------------------------------------------------------------
bool InitGeometry()
{
    try
    {
        gCubeMesh = new Mesh("Models/Cube.x");
        gFloorMesh = new Mesh("Models/Hills.x");
        gLightMesh = new Mesh("Models/Light.x");
        gTeaPotMesh = new Mesh("Models/TeaPot.x");
    }
    catch (std::runtime_error e)
    {
        gLastError = e.what();
        return false;
    }

    if (!LoadShaders())
    {
        gLastError = "Error loading shaders";
        return false;
    }

    gPerFrameConstantBuffer = CreateConstantBuffer(sizeof(gPerFrameConstants));
    gPerModelConstantBuffer = CreateConstantBuffer(sizeof(gPerModelConstants));
    if (gPerFrameConstantBuffer == nullptr || gPerModelConstantBuffer == nullptr)
    {
        gLastError = "Error creating constant buffers";
        return false;
    }

    if (!LoadTexture("Textures/StoneDiffuseSpecular.dds", &gCubeDiffuseMap, &gCubeDiffuseMapSRV) ||
        !LoadTexture("Textures/WoodDiffuseSpecular.dds", &gFloorDiffuseMap, &gFloorDiffuseMapSRV) ||
        !LoadTexture("Textures/StoneDiffuseSpecular.dds", &gTeaPotDiffuseMap, &gTeaPotDiffuseMapSRV) ||
        !LoadTexture("Textures/Flare.jpg", &gLightDiffuseMap, &gLightDiffuseMapSRV))
    {
        gLastError = "Error loading textures";
        return false;
    }

    if (!CreateStates())
    {
        gLastError = "Error creating states";
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------
// InitScene
//--------------------------------------------------------------------------------------
bool InitScene()
{
    gCube = new Model(gCubeMesh);
    gCube2 = new Model(gCubeMesh);  // тот же меш, другая позиция
    gFloor = new Model(gFloorMesh);
    gTeaPot = new Model(gTeaPotMesh);

    gCube->SetPosition({ 0.0f, 3.0f,  0.0f });
    gCube->SetScale(1.0f);

    gCube2->SetPosition({ 10.0f, 3.0f, 0.0f });
    gCube2->SetScale(1.0f);

    gTeaPot->SetPosition({ 5.0f, 6.0f, 0.0f });
    gTeaPot->SetScale(0.8f);

    gFloor->SetPosition({ 0.0f, 0.0f, 0.0f });

    gLights[0].model = new Model(gLightMesh);
    gLights[0].colour = { 1.0f, 0.8f, 0.4f };
    gLights[0].strength = 10.0f;
    gLights[0].model->SetPosition({ 0.0f, 10.0f, 0.0f });
    gLights[0].model->SetScale(pow(gLights[0].strength, 0.7f));

    gCamera = new Camera();
    gCamera->SetPosition({ -3.0f, 5.0f, -10.0f });
    gCamera->SetRotation({ ToRadians(13.0f), ToRadians(15.0f), 0.0f });

    return true;
}

//--------------------------------------------------------------------------------------
// ReleaseResources
//--------------------------------------------------------------------------------------
void ReleaseResources()
{
    ReleaseStates();

    if (gLightDiffuseMapSRV)   gLightDiffuseMapSRV->Release();
    if (gLightDiffuseMap)      gLightDiffuseMap->Release();
    if (gTeaPotDiffuseMapSRV)  gTeaPotDiffuseMapSRV->Release();
    if (gTeaPotDiffuseMap)     gTeaPotDiffuseMap->Release();
    if (gFloorDiffuseMapSRV)   gFloorDiffuseMapSRV->Release();
    if (gFloorDiffuseMap)      gFloorDiffuseMap->Release();
    if (gCubeDiffuseMapSRV)    gCubeDiffuseMapSRV->Release();
    if (gCubeDiffuseMap)       gCubeDiffuseMap->Release();

    if (gPerModelConstantBuffer) gPerModelConstantBuffer->Release();
    if (gPerFrameConstantBuffer) gPerFrameConstantBuffer->Release();

    ReleaseShaders();

    for (int i = 0; i < NUM_LIGHTS; ++i) { delete gLights[i].model; gLights[i].model = nullptr; }
    delete gCamera;  gCamera = nullptr;
    delete gTeaPot;  gTeaPot = nullptr;
    delete gCube2;   gCube2 = nullptr;
    delete gCube;    gCube = nullptr;
    delete gFloor;   gFloor = nullptr;

    delete gTeaPotMesh;  gTeaPotMesh = nullptr;
    delete gLightMesh;   gLightMesh = nullptr;
    delete gFloorMesh;   gFloorMesh = nullptr;
    delete gCubeMesh;    gCubeMesh = nullptr;
}

//--------------------------------------------------------------------------------------
// Scene Rendering
//--------------------------------------------------------------------------------------
void RenderSceneFromCamera(Camera* camera)
{
    gPerFrameConstants.viewMatrix = camera->ViewMatrix();
    gPerFrameConstants.projectionMatrix = camera->ProjectionMatrix();
    gPerFrameConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(gPerFrameConstantBuffer, gPerFrameConstants);

    gD3DContext->VSSetConstantBuffers(0, 1, &gPerFrameConstantBuffer);
    gD3DContext->PSSetConstantBuffers(0, 1, &gPerFrameConstantBuffer);

    //// Обычные модели ////
    gD3DContext->VSSetShader(gPixelLightingVertexShader, nullptr, 0);
    gD3DContext->PSSetShader(gPixelLightingPixelShader, nullptr, 0);
    gD3DContext->OMSetBlendState(gNoBlendingState, nullptr, 0xffffff);
    gD3DContext->OMSetDepthStencilState(gUseDepthBufferState, 0);
    gD3DContext->RSSetState(gCullBackState);
    gD3DContext->PSSetSamplers(0, 1, &gAnisotropic4xSampler);

    gD3DContext->PSSetShaderResources(0, 1, &gFloorDiffuseMapSRV);
    gFloor->Render();

    gD3DContext->PSSetShaderResources(0, 1, &gCubeDiffuseMapSRV);
    gCube->Render();
    gCube2->Render();

    gD3DContext->PSSetShaderResources(0, 1, &gTeaPotDiffuseMapSRV);
    gTeaPot->Render();

    //// Свет ////
    gD3DContext->VSSetShader(gBasicTransformVertexShader, nullptr, 0);
    gD3DContext->PSSetShader(gLightModelPixelShader, nullptr, 0);
    gD3DContext->PSSetShaderResources(0, 1, &gLightDiffuseMapSRV);
    gD3DContext->OMSetBlendState(gAdditiveBlendingState, nullptr, 0xffffff);
    gD3DContext->OMSetDepthStencilState(gDepthReadOnlyState, 0);
    gD3DContext->RSSetState(gCullNoneState);

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        gPerModelConstants.objectColour = gLights[i].colour;
        gLights[i].model->Render();
    }
}

void RenderScene()
{
    gPerFrameConstants.light1Colour = gLights[0].colour * gLights[0].strength;
    gPerFrameConstants.light1Position = gLights[0].model->Position();
    gPerFrameConstants.ambientColour = gAmbientColour;
    gPerFrameConstants.specularPower = gSpecularPower;
    gPerFrameConstants.cameraPosition = gCamera->Position();

    gD3DContext->OMSetRenderTargets(1, &gBackBufferRenderTarget, gDepthStencil);
    gD3DContext->ClearRenderTargetView(gBackBufferRenderTarget, &gBackgroundColor.r);
    gD3DContext->ClearDepthStencilView(gDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(gViewportWidth);
    vp.Height = static_cast<FLOAT>(gViewportHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    gD3DContext->RSSetViewports(1, &vp);

    RenderSceneFromCamera(gCamera);

    gSwapChain->Present(lockFPS ? 1 : 0, 0);
}

//--------------------------------------------------------------------------------------
// Scene Update
//--------------------------------------------------------------------------------------
void UpdateScene(float frameTime)
{
    // Управление кубом
    gCube->Control(0, frameTime, Key_I, Key_K, Key_J, Key_L, Key_U, Key_O, Key_Period, Key_Comma);

    // Орбита света вокруг куба
    static float rotate = 0.0f;
    gLights[0].model->SetPosition(gCube->Position() + CVector3{ cos(rotate) * gLightOrbit, 10.0f, sin(rotate) * gLightOrbit });
    rotate -= gLightOrbitSpeed * frameTime;

    // Камера
    gCamera->Control(frameTime, Key_Up, Key_Down, Key_Left, Key_Right, Key_W, Key_S, Key_A, Key_D);

    if (KeyHit(Key_P)) lockFPS = !lockFPS;

    const float fpsUpdateTime = 0.5f;
    static float totalFrameTime = 0;
    static int frameCount = 0;
    totalFrameTime += frameTime;
    ++frameCount;
    if (totalFrameTime > fpsUpdateTime)
    {
        float avgFrameTime = totalFrameTime / frameCount;
        std::ostringstream frameTimeMs;
        frameTimeMs.precision(2);
        frameTimeMs << std::fixed << avgFrameTime * 1000;
        std::string windowTitle = "Nikita Markov G21071959: " + frameTimeMs.str() +
            "ms, FPS: " + std::to_string(static_cast<int>(1 / avgFrameTime + 0.5f));
        SetWindowTextA(gHWnd, windowTitle.c_str());
        totalFrameTime = 0;
        frameCount = 0;
    }
}
