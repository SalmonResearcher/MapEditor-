#pragma once
#include "Engine/GameObject.h"

const int MODEL_NUM{ 5 };
const int XSIZE{ 15 };
const int ZSIZE{ 15 };

enum BLOCKTYPE {
    DEFAULT,BRICK,GRASS,SAND,WATER,MAX
};

//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_[MAX];    //モデル番号

    struct Blockinfo {
        BLOCKTYPE type_;
        int height;
    };
    Blockinfo table_[XSIZE][ZSIZE];

public:
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};