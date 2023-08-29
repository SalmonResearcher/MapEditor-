#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"

//コンストラクタ
Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller")
{
}

//デストラクタ
Controller::~Controller()
{
}

//初期化
void Controller::Initialize()
{

	
}

//更新
void Controller::Update()
{
	XMVECTOR nowVec = XMLoadFloat3(&transform_.position_);
	XMVECTOR move = { 0,0,speed, 0 };
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));


	if (Input::IsKey(DIK_Q))
	{
		transform_.rotate_.y += 1;
	}

	if (Input::IsKey(DIK_E))
	{
		transform_.rotate_.y -= 1;

	}
	if (Input::IsKey(DIK_A)) 
	{
		XMVECTOR a = XMVectorSet(0, 1, 0, 0);
		nowVec += XMVector3Cross(move, a);

	}
	if (Input::IsKey(DIK_D))
	{
		XMVECTOR a = XMVectorSet(0, 1, 0, 0);
		nowVec -= XMVector3Cross(move,a);
	}
	if (Input::IsKey(DIK_W))
	{
		nowVec += move;
		XMStoreFloat3(&transform_.position_, nowVec);
	}
	if (Input::IsKey(DIK_S))
	{
		nowVec -= move;
		XMStoreFloat3(&transform_.position_, nowVec);
	}

	XMStoreFloat3(&transform_.position_, nowVec);

	XMVECTOR vCam = { 0,5,-15,0 };
	Camera::SetTarget(transform_.position_);
	vCam = XMVector3TransformCoord(vCam, rotMat);
	XMStoreFloat3(&Cam.position_, nowVec + vCam);
	Camera::SetPosition(Cam.position_);

}

//描画
void Controller::Draw()
{

}

//開放
void Controller::Release()
{
}