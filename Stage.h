#pragma once
#include "Engine/GameObject.h"

const int MODEL_NUM{ 5 };
const int XSIZE{ 15 };
const int ZSIZE{ 15 };

enum BLOCKTYPE {
    DEFAULT,BRICK,GRASS,SAND,WATER
};

//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_[5];    //モデル番号
    int table_[XSIZE][ZSIZE];

    struct BlockSet {
        int BLOCKTYPE;
        int height;
        int table_[XSIZE][ZSIZE];
    };

public:
    void SetBrock(int _x, int _z, BLOCKTYPE _type);
    void SetBrockHeight(int _x, int _z, int height);
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