#include "Stage.h"
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