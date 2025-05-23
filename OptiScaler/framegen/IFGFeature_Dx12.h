#pragma once
#include <pch.h>
#include "IFGFeature.h"

#include <upscalers/IFeature.h>

#include <dxgi1_6.h>
#include <d3d12.h>

class IFGFeature_Dx12 : public virtual IFGFeature
{
protected:
    IDXGISwapChain* _swapChain = nullptr;
    HWND _hwnd = NULL;
    ID3D12CommandQueue* _gameCommandQueue = nullptr;

    ID3D12Resource* _paramVelocity[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Resource* _paramVelocityCopy[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Resource* _paramDepth[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Resource* _paramDepthCopy[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Resource* _paramHudless[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Resource* _paramHudlessCopy[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };

    ID3D12CommandQueue* _commandQueue = nullptr;
    ID3D12GraphicsCommandList* _commandList[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12CommandAllocator* _commandAllocators[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Fence* _fgFence = nullptr;

    ID3D12CommandQueue* _copyCommandQueue = nullptr;
    ID3D12GraphicsCommandList* _copyCommandList[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12CommandAllocator* _copyCommandAllocator[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Fence* _copyFence = nullptr;

    ID3D12CommandQueue* _hudlessCommandQueue = nullptr;
    ID3D12GraphicsCommandList* _hudlessCommandList[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12CommandAllocator* _hudlessCommandAllocator[BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
    ID3D12Fence* _hudlessFence = nullptr;
    ID3D12Fence* _hudlessCopyFence = nullptr;

    bool CreateBufferResource(ID3D12Device* InDevice, ID3D12Resource* InSource, D3D12_RESOURCE_STATES InState, ID3D12Resource** OutResource);
    void ResourceBarrier(ID3D12GraphicsCommandList* InCommandList, ID3D12Resource* InResource, D3D12_RESOURCE_STATES InBeforeState, D3D12_RESOURCE_STATES InAfterState);
    bool CopyResource(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* source, ID3D12Resource** target, D3D12_RESOURCE_STATES sourceState);

public:

    void SetVelocity(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* velocity, D3D12_RESOURCE_STATES state);
    void SetDepth(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* depth, D3D12_RESOURCE_STATES state);
    void SetHudless(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* hudless, D3D12_RESOURCE_STATES state, bool makeCopy = false);

    virtual bool CreateSwapchain(IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, DXGI_SWAP_CHAIN_DESC* desc, IDXGISwapChain** swapChain) = 0;
    virtual bool CreateSwapchain1(IDXGIFactory* factory, ID3D12CommandQueue* cmdQueue, HWND hwnd, DXGI_SWAP_CHAIN_DESC1* desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGISwapChain1** swapChain) = 0;
    virtual bool ReleaseSwapchain(HWND hwnd) = 0;
    
    void CreateObjects(ID3D12Device* InDevice);
    virtual void CreateContext(ID3D12Device* device, IFeature* upscalerContext) = 0;
    void ReleaseObjects() final;

    ID3D12Fence* GetCopyFence();
    ID3D12Fence* GetHudlessFence();
    
    void SetWaitOnGameQueue(UINT64 value);

    virtual bool Dispatch(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* output, double frameTime) = 0;
    virtual bool DispatchHudless(bool useHudless, double frameTime) = 0;

    bool IsFGCommandList(void* cmdList);

    IFGFeature_Dx12() = default;
};