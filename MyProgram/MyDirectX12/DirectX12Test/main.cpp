#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <fstream>
//#include <windows.h>

using namespace std;
// Viewport dimensions.
UINT windowWidth = 800;
UINT windowHeight = 800;
float m_aspectRatio;
wstring windowName = L"First Window.";


void OnInit();
void OnUpdate();
void OnRender();
void OnDestroy();
bool OnEvent(MSG msg);


void GetHardwareAdapter(_In_ IDXGIFactory4* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int Run(HINSTANCE hInstance, int nCmdShow);


// Window handle.
HWND m_hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	return Run(hInstance, nCmdShow);
}
// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void GetHardwareAdapter(_In_ IDXGIFactory4* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter)
{
	IDXGIAdapter1* pAdapter = nullptr;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &pAdapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		pAdapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = pAdapter;
}

// Main message handler for the sample.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Handle destroy/shutdown messages.
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int Run(HINSTANCE hInstance, int nCmdShow)
{
	// Initialize the window class.
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"WindowClass1";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	m_hwnd = CreateWindowEx(NULL,
		L"WindowClass1",
		windowName.c_str(),
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,		// We have no parent window, NULL.
		NULL,		// We aren't using menus, NULL.
		hInstance,
		NULL);		// We aren't using multiple windows, NULL.

	ShowWindow(m_hwnd, nCmdShow);

	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	OnInit();

	// Main sample loop.
	MSG msg = { 0 };
	while (true)
	{
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;

			// Pass events into our sample.
			OnEvent(msg);
		}

		OnUpdate();
		OnRender();
	}

	OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	return static_cast<char>(msg.wParam);
}

void OnInit()
{
}
void OnUpdate()
{
}
void OnRender()
{
}
void OnDestroy()
{
}
bool OnEvent(MSG msg)
{
	return true;
}
