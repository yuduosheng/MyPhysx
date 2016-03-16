#ifndef DX12APP_H_
#define DX12APP_H_
#include "UTIL.h"

class MYAPP
{
public:
	MYAPP(HINSTANCE hInstance, int nCmdShow, std::wstring windowName, UINT windowWidth, UINT windowHeight, UINT FrameCount);
	virtual ~MYAPP() {}


	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	// render relate
	virtual bool Init() = 0;
	virtual void UpdateScene() = 0;
	virtual void Render() = 0;
	// initiate
	// Main message handler for the sample.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	virtual void initMainWindow(HINSTANCE hInstance, int nCmdShow);
	virtual void initDX();
	virtual int Run();
	virtual bool OnEvent(MSG msg);
	virtual void OnDestroy();

	void WaitForPreviousFrame();

	HINSTANCE getAppInst() { return _appInst; }
	HWND      getMainWnd() { return _hwnd; }

protected:
	void LoadPipeline();
	
	// Window handle.
	HINSTANCE  _appInst;//³ÌÐòÊµÀý
	HWND _hwnd;
	// Viewport dimensions.
	UINT windowWidth;
	UINT windowHeight;
	std::wstring windowName;
	float _aspectRatio;
	// Pipeline objects.
	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;
	const UINT FrameCount;
	ComPtr<IDXGISwapChain3> _swapChain;
	ComPtr<ID3D12Device> _device;
	ComPtr<ID3D12Resource> *_renderTargets;
	ComPtr<ID3D12CommandAllocator> _commandAllocator;
	ComPtr<ID3D12CommandQueue> _commandQueue;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	ComPtr<ID3D12GraphicsCommandList> _commandList;
	UINT _rtvDescriptorSize;
	//assets objects
	ComPtr<ID3D12RootSignature> _rootSignature;
	ComPtr<ID3D12PipelineState> _pipelineState;
	// App resources.
	ComPtr<ID3D12Resource> _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
	// Synchronization objects.
	UINT _frameIndex;
	HANDLE _fenceEvent;
	ComPtr<ID3D12Fence> _fence;
	UINT64 _fenceValue;
};

#endif