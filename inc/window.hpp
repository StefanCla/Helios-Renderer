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

class Window
{
public:
	Window(HINSTANCE hInst, const wchar_t* windowClassName);
	~Window();

	// Register the window to the OS, so they know a window is present
	//void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName);

	// Create the window itself
	void CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height);

	// Set the window to fullscreen or windowed
	void SetFullScreen(bool bFullscreen);

	// Create Swap Chain using the command queue
	void CreateSwapChain(Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue, bool bTearingSupported);

	// Create Descriptor Heap using device
	void CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type);

	void UpdateRenderTargetView(Microsoft::WRL::ComPtr<ID3D12Device2> device);

	void Update();
	void ShowWindow();

	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource[g_BufferCount];

	UINT m_DescriptorSize;

protected:
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HWND m_HWnd;
	RECT m_WindowRect;

	bool m_bVSync = true;
	bool m_bFullscreen = false;

};