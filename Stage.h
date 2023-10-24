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

//Stage���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //���f���ԍ�

    struct Blockinfo {
        int type;
        int height;
    } table_[XSIZE][ZSIZE];

    int mode_;      //�グ����,��ނ̕ύX
    int select_;    //���
    bool bold_;     //��m�i

    int curBrockX = -1;
    int curBrockZ = -1;
    int nowBrockX = -1;
    int nowBrockZ = -1;

public:
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    void Save();
    void Load();


};