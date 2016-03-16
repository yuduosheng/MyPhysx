#include "DirectInputClass.h"
#include "DX12APP.h"
extern MYAPP *_app;
DirectInput* gDInput = 0;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	//��ʼ��һ��IDirectInput8�ӿڶ���
	ThrowIfFailed(DirectInput8Create(_app->getAppInst(), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&mDInput, 0));
	//���м����豸�ĳ�ʼ��
	ThrowIfFailed(mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0));
	ThrowIfFailed(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	ThrowIfFailed(mKeyboard->SetCooperativeLevel(_app->getMainWnd(), keyboardCoopFlags));
	ThrowIfFailed(mKeyboard->Acquire());
	//��������豸�ĳ�ʼ��
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
	// ��ѯ����
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	if (FAILED(hr))
	{
		// ��ʧ���̣��ü���״̬����Ϊ��
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		// ��ͼ��ȡ����
		hr = mKeyboard->Acquire();
	}

	// ��ѯ���
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