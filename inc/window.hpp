#pragma once
#include <Windows.h>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_5.h>

// Reponsible for creating the Windows window.
// Should allow for resize & fullscreen, handle input as well (for now).

#if defined(CreateWindow)
#undef CreateWindow
#endif

class CommandQueue;

class Window
{
public:
	Window(HINSTANCE hInst, const wchar_t* windowClassName);
	~Window();

	// Create the window itself
	void CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height);

	// Set the window to fullscreen or windowed
	void SetFullScreen(bool bFullscreen);
	void ToggleFullScreen();

	// Set & Get VSync
	void ToggleVSync();
	const bool GetVSync() const;

	// Create Swap Chain using the command queue
	void CreateSwapChain(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, bool bTearingSupported);

	// Create Descriptor Heap using device
	void CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type);

	// Update the window render target view
	void UpdateRenderTargetView(Microsoft::WRL::ComPtr<ID3D12Device2> device);

	// Update the window, does not render the image to screen
	void Update();

	// Show window
	void ShowWindow();

	// Hide window
	void HideWindow();

	// Resize the window properly
	void Resize(uint32_t width, uint32_t height, std::shared_ptr<CommandQueue> commandQueue);

	// Getters
	const Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain() const { return m_SwapChain; }
	const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return m_DescriptorHeap; }
	const Microsoft::WRL::ComPtr<ID3D12Resource> GetResource(const uint32_t backBufferIndex) const;

public:
	UINT m_DescriptorSize;

protected:
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource[g_BufferCount];

	HWND m_HWnd;
	RECT m_WindowRect;

	bool m_bVSync = true;
	bool m_bFullscreen = false;
};