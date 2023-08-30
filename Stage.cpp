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
				table_[x][z] = -1;
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
			table_[x][z] = x % 5;
		}
	}
}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform Block;
	
	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < BlockSet::height; y++)
			{
				int type = table_[x][z];
				type = 4;

				Block.position_.x = x;
				Block.position_.z = z;

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