#include "commandqueue.hpp"
#include "helperfunc.hpp"
#include "window.hpp"

using namespace Microsoft::WRL;

CommandQueue::CommandQueue(ComPtr<ID3D12Device2> device)
	: m_Device(device)
{
	CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);

    for (int i = 0; i < 3; i++)
    {
        m_CommandAllocators[i] = CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);
    }

	CreateCommandList(m_CommandAllocators[m_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

    CreateEventHandle();
	CreateFence();

	// Initialize here
}

CommandQueue::~CommandQueue()
{
	// De-initialize here
}

void CommandQueue::CreateFence()
{
	ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
}

void CommandQueue::CreateEventHandle()
{
	m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(m_FenceEvent && "Failed to create fence event.");
}

void CommandQueue::Signal(ComPtr<ID3D12CommandQueue> commandQueue, uint64_t& fenceValue)
{
	m_FenceValue = ++fenceValue;
	ThrowIfFailed(commandQueue->Signal(m_Fence.Get(), m_FenceValue));
}

void CommandQueue::WaitForFenceValue(uint64_t fenceValue)
{
	std::chrono::milliseconds duration = std::chrono::milliseconds::max();

	if (m_Fence->GetCompletedValue() < m_FenceValue)
	{
		ThrowIfFailed(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
		::WaitForSingleObject(m_FenceEvent, static_cast<DWORD>(duration.count()));
	}
}

void CommandQueue::Flush(ComPtr<ID3D12CommandQueue> commandQueue)
{
	Signal(commandQueue, m_FenceValue);
	WaitForFenceValue(m_FenceValue);
}

void CommandQueue::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(m_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));
}

ComPtr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type)
{
    ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(m_Device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

    return commandAllocator;
}

void CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
	ThrowIfFailed(m_Device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));

	ThrowIfFailed(m_CommandList->Close());
}

void CommandQueue::Render(std::shared_ptr<Window> window)
{
    auto commandAllocator = m_CommandAllocators[m_CurrentBackBufferIndex];
    auto backBuffer = window->m_Resource[m_CurrentBackBufferIndex];

    commandAllocator->Reset();
    m_CommandList->Reset(commandAllocator.Get(), nullptr);

    // Clear the render target.
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

        m_CommandList->ResourceBarrier(1, &barrier);

        FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(window->m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            m_CurrentBackBufferIndex, window->m_DescriptorSize);

        m_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_CommandList->ResourceBarrier(1, &barrier);

        ThrowIfFailed(m_CommandList->Close());

        ID3D12CommandList* const commandLists[] = {
            m_CommandList.Get()
        };
        m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

        Signal(m_CommandQueue, m_FenceValue);
        m_FrameFenceValues[m_CurrentBackBufferIndex] = m_FenceValue;

        UINT syncInterval = false; // g_VSync ? 1 : 0;
        UINT presentFlags = false; // g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
        ThrowIfFailed(window->m_SwapChain->Present(syncInterval, presentFlags));

        m_CurrentBackBufferIndex = window->m_SwapChain->GetCurrentBackBufferIndex();

        WaitForFenceValue(m_FrameFenceValues[m_CurrentBackBufferIndex]);
    }
}