#pragma once

#include <dInput.h>
#include <DirectXMath.h>
#include "Direct3D.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
using namespace DirectX;

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	void Release();

	//É}ÉEÉX
	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);
	XMFLOAT3 GetMousePosition();
	XMFLOAT3 GetMouseMove();
	void SetMousePosition(int x, int y);
};