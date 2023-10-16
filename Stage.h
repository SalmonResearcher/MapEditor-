#pragma once
#include "Engine/GameObject.h"

const int MODEL_NUM{ 5 };
const int XSIZE{ 15 };
const int ZSIZE{ 15 };

enum BLOCKTYPE {
    DEFAULT,BRICK,GRASS,SAND,WATER,MAX
};

//Stage���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[MAX];    //���f���ԍ�

    struct Blockinfo {
        BLOCKTYPE type_;
        int height;
    } table_[XSIZE][ZSIZE];

    int mode_;      //�グ����
    int select_;    //���

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