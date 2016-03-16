#include "DirectInputClass.h"
#include "DX12APP.h"
extern MYAPP *_app;
DirectInput* gDInput = 0;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	//初始化一个IDirectInput8接口对象
	ThrowIfFailed(DirectInput8Create(_app->getAppInst(), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mDInput, 0));
	//进行键盘设备的初始化
	ThrowIfFailed(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	ThrowIfFailed(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	ThrowIfFailed(mKeyboard->SetCooperativeLevel(_app->getMainWnd(), keyboardCoopFlags));
	ThrowIfFailed(mKeyboard->Acquire());
	//进行鼠标设备的初始化
	ThrowIfFailed(mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0));
	ThrowIfFailed(mMouse->SetDataFormat(&c_dfDIMouse2));
	ThrowIfFailed(mMouse->SetCooperativeLevel(_app->getMainWnd(), mouseCoopFlags));
	ThrowIfFailed(mMouse->Acquire());
}

DirectInput::~DirectInput()
{
	SAFE_RELEASE(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	SAFE_RELEASE(mKeyboard);
	SAFE_RELEASE(mMouse);
}

void DirectInput::poll()
{
	// 轮询键盘
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	if (FAILED(hr))
	{
		// 丢失键盘，置键盘状态数组为零
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		// 试图获取键盘
		hr = mKeyboard->Acquire();
	}

	// 轮询鼠标
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&mMouseState, sizeof(mMouseState));
		hr = mMouse->Acquire();
	}
}

bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}