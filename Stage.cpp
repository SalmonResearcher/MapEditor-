#include "Stage.h"
#include "Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_(-1)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
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
			int type = x % 2;

			Block.position_.x = x;
			Block.position_.z = z;

			Model::SetTransform(hModel_, Block);
			Model::Draw(hModel_);
		}
	}
}

//開放
void Stage::Release()
{
}