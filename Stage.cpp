#include "Stage.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_{ -1, -1, -1, -1, -1 }
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		hModel_[i] = -1;
		for (int x = 0; x < XSIZE; x++)
		{
			for (int z = 0; z < ZSIZE; z++)
				table_[x][z].type_ = DEFAULT;
		}
	}
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
	string modelName[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxGrass.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx"
	};
	string filename_base = "assets/";
	srand(time(NULL));

    //モデルデータのロード
	for (int i = 0; i < 5; i++) 
	{
		hModel_[i] = Model::Load(filename_base+modelName[i]);
		assert(hModel_[i] >= 0);
	}
	for (int z = 0; z < ZSIZE; z++)
	{
		for (int x = 0; x < XSIZE; x++)
		{
			table_[x][z].type_ = BRICK;
			SetBlockHeight(x, z, rand() % 20 + 1);
		}
	}
	SetBlock(1, 1, SAND);
}

//更新
void Stage::Update()
{
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();

	XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
	XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][y].height + 1; y++)
			{
				RayCastData data;
				XMStoreFloat4(&data.start, vMouseFront);
				XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);
				Model::RayCast(hModel_[0], data);

				if (data.hit)
				{
					table_[x][z].height++;
					break;
				}
			}
		}

	}
}

//描画
void Stage::Draw()
{
    Transform Block;
	
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			for (int y = 0; y < table_[x][y].height; y++)
			{
				int type = table_[x][z].type_;

				Block.position_.x = x ;
				Block.position_.z = z ;
				Block.position_.y = y;

				Model::SetTransform(hModel_[type], Block);
				Model::Draw(hModel_[type]);
			}
		}
	}
}

//開放
void Stage::Release()
{
}

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type_ = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
}