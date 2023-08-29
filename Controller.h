#pragma once
#include "Engine/GameObject.h"

class Controller:public GameObject
{
    XMFLOAT3 Camposition_ = { 7,7,0 };


    const float speed = 0.125;
    Transform Cam;
    Transform Tar;
public:
    //�R���X�g���N�^
    Controller(GameObject * parent);

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};