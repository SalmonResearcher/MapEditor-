#pragma once
#include "Engine/GameObject.h"
namespace {
    const int MODEL_NUM{ 5 };
    const int XSIZE{ 15 };
    const int ZSIZE{ 15 };
    enum BLOCKTYPE
    {
        DEFAULT, BRICL, GRASS, SAND, WATER
    };
}

//Stageを管理するクラス
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //モデル番号

    struct Blockinfo {
        int type;
        int height;
    } table_[XSIZE][ZSIZE];

    int mode_;      //上げ下げ,種類の変更
    int select_;    //種類
    bool bold_;     //大卜司

    int curBrockX = -1;
    int curBrockZ = -1;
    int nowBrockX = -1;
    int nowBrockZ = -1;

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
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    void Save();
    void Load();


};