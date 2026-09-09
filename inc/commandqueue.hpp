#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <directx/d3dx12.h>

// Responsible for handling the command queue
// Should also set the fences to indicate the end of a queue.
class Window;

class CommandQueue
{
public:
	CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device);
	~CommandQueue();

	// Create a fence to add to the command queue
	void CreateFence();

	// Create an event handle to handle when the fence event has been called
	void CreateEventHandle();

	// Signal when the fence has been reached
	void Signal(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, uint64_t& fenceValue);

	// Stall the CPU to wait for the fence value to be reached
	void WaitForFenceValue(uint64_t fenceValue);

	// Flush the commandqueu 
	void Flush(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue);

	// Create command queue for fence synchronization
	void CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type);

	// Create Command Allocator: Allocate space for the command queue commands
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type);

	// Create Command List using device: The commands to be executed
	void CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);

	void Render(std::shared_ptr<Window> window);

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue = nullptr;

private:
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence = nullptr;
	HANDLE m_FenceEvent;
	uint64_t m_FenceValue = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators[3];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Device2> m_Device = nullptr;

	UINT m_CurrentBackBufferIndex = 0;
	uint64_t m_FrameFenceValues[3] = {};
};