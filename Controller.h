#pragma once
#include "Engine/GameObject.h"

class Controller:public GameObject
{
    XMFLOAT3 Camposition_ = { 7,7,0 };


    const float speed = 0.125;
    Transform Cam;
    Transform Tar;
public:
    //コンストラクタ
    Controller(GameObject * parent);

    //デストラクタ
    ~Controller();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};