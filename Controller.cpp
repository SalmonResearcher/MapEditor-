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
	transform_.rotate_.x = -10;
	transform_.position_.z = -5;
}

//更新
void Controller::Update()
{
	//ここから回転させます
	if (Input::IsKey(DIK_Q))
	{
		transform_.rotate_.y += 1;
	}

	if (Input::IsKey(DIK_E))
	{
		transform_.rotate_.y -= 1;
	}

	if (Input::IsKey(DIK_F))
	{
		//内積を取ってない
		transform_.rotate_.x += 1;
		if (transform_.rotate_.x >= 69)
		{
			transform_.rotate_.x = 69;
		}
	}

	if (Input::IsKey(DIK_C))
	{
		//内積をとってない
		transform_.rotate_.x -= 1;
		if (transform_.rotate_.x <= -20)
		{
			transform_.rotate_.x = -20;
		}
	}

	//回転行列
	XMMATRIX rotMatY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

	//rotateMatrix
	XMMATRIX rotMatX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));

	//移動ベクトル
	XMVECTOR nowVec = XMLoadFloat3(&transform_.position_);
	XMVECTOR frontMove = XMVectorSet( 0,0,speed, 0 );
	frontMove = XMVector3TransformCoord(frontMove, rotMatY);

	//上下左右回転
	XMVECTOR udMove = XMVectorSet(0, speed, 0, 0);
	udMove = XMVector3TransformCoord(udMove, rotMatY);
	XMVECTOR RLMove = XMVectorSet(speed, 0, 0, 0);
	RLMove = XMVector3TransformCoord(RLMove, rotMatY);

	//右に移動
	if (Input::IsKey(DIK_A)) 
	{
		nowVec -= RLMove;
	}

	//左に移動
	if (Input::IsKey(DIK_D))
	{
		nowVec += RLMove;
	}

	//前進
	if (Input::IsKey(DIK_W))
	{
		nowVec += frontMove;
	}

	//後退
	if (Input::IsKey(DIK_S))
	{
		nowVec -= frontMove;
	}

	//上昇
	if (Input::IsKey(DIK_LCONTROL))
	{
		nowVec -= udMove;
	}
	//下降
	if (Input::IsKey(DIK_SPACE))
	{
		nowVec += udMove;
	}

	//カメラ移動
	XMStoreFloat3(&transform_.position_, nowVec);

	//カメラ本体
	XMVECTOR vCam = { 0,5,-15,0 };

	//カメラ注視点
	Camera::SetTarget(transform_.position_);
	vCam = XMVector3TransformCoord(vCam, rotMatX * rotMatY);
	
	//カメラ座標変更
	XMStoreFloat3(&Camposition_, nowVec + vCam);
	Camera::SetPosition(Camposition_);

}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}