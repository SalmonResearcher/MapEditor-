#include "Input.h"
#include <string>

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256];    //前フレームでの各キーの状態

	LPDIRECTINPUTDEVICE8 pMouseDevice_ = nullptr;
	DIMOUSESTATE mouseState_;
	DIMOUSESTATE prevMouseState_;
	XMFLOAT3 mousePosition_;


	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(prevKeyState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//今は押してなくて、前回は押してる
		if (!IsKey(keyCode) && prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
	}

	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}
	//マウスカーソルの位置を取得
	XMFLOAT3 GetMousePosition()
	{
		return mousePosition_;
	}

	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX,
									(float)mouseState_.lY,
									(float)mouseState_.lZ);
		return result;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition_.x = x;
		mousePosition_.y = y;
		std::string resStr = std::to_string(x) + "," + std::to_string(y);
		OutputDebugString(resStr.c_str());
	}
}