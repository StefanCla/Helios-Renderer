#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <directx/d3dx12.h>

// Responsible for initializing the renderer.
// Should find the GPU, create the device and set up the everything.

// We can only have 1 Application active, as such, this will be a singleton.

class CommandQueue;
class Window;

class Application
{
public:
	static void Create(HINSTANCE hInst);

	static Application& Get();

	// Enable debug layer, must be done before creating the device
	void EnableDebugLayer();

	// Query for what adapter to use
	void QueryAdapter(bool bUseWarp);

	// Create Device with queried adapter
	void CreateDevice();

	// Check if tearing is supported
	void CheckTearingSupport();

	void Application::RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName);

	std::shared_ptr<CommandQueue> m_CommandQueue;
	std::shared_ptr<Window> m_Window;

private:
	Application(HINSTANCE hInst);
	Application(Application const&) = delete;
	Application& operator= (Application const&) = delete;

	~Application();

private:
	Microsoft::WRL::ComPtr<ID3D12Debug> m_DebugInterface = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> m_Adapter = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device2> m_Device = nullptr;

	bool m_bTearingSupported = false;

	HINSTANCE m_HInstance;
};