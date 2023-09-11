#include "Model.h"
#include "Direct3D.h"

namespace Model {

	//���f���̃|�C���^���Ԃ�����ł����x�N�^
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string fileName)
{
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = fileName;
	pData->pfbx_ = nullptr;

	//�t�@�C���l�[����������������A�ǂ܂�I
	for (auto& e : modelList)
	{
		if (e->filename_ == fileName) {
			pData->pfbx_ = e->pfbx_;
			break;
		}
	}

	if (pData->pfbx_ == nullptr)
	{
		pData->pfbx_ = new Fbx;
		pData->pfbx_->Load(fileName);
	}

	modelList.push_back(pData);
	return( modelList.size() - 1 );
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
	//���f���ԍ��́AmodelList�̃C���f�b�N�X
}
void Model::Draw(int hModel) {
	//���f���ԍ��́AmodelList�̃C���f�b�N�X
	modelList[hModel]->pfbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false; //�Q�Ƃ���Ă�H
	for (int i=0; i < modelList.size(); i++)
	{
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->pfbx_ == modelList[j]->pfbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pfbx_);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear();
}

void Model::RayCast(int hModel, RayCastData& raydata)
{
	//���f���̃g�����X�t�H�[�����J���L�����[�V����
	modelList[hModel]->transform_.Calclation();
	//�@���[���h�s��̋t�s��
	XMMATRIX wInv = XMMatrixInverse(nullptr,
					modelList[hModel]->transform_.GetWorldMatrix());
	//�A���C�̒ʉߓ_�����߂�i���f����Ԃł̃��C�̕����x�N�g�������߂�j
	XMVECTOR vpass{ raydata.start.x + raydata.dir.x,
					raydata.start.y + raydata.dir.y,
					raydata.start.z + raydata.dir.z,
					raydata.start.w + raydata.dir.w };
	//�BrayData.stat�����f����Ԃɕϊ��i�@���|����j
	XMVECTOR vstart = XMLoadFloat4(&raydata.start);
	vstart = XMVector3TransformCoord(vstart, wInv); //tarnsformcoord��w�v�f�𖳎����Ă����炵��
	XMStoreFloat4(&raydata.start, vstart); //����@�l���Z�b�g����̖Y��Ă��B�B�B

	//�C�i�n�_��������x�N�g�������傢�L�΂�����j�ʉߓ_�i�A�j�ɇ@���|����
	vpass = 
	//�DrayData.dir���B����C�Ɍ������x�N�g���ɂ���i�����Z�j
	vpass = vpass - vstart;
	XMStoreFloat4(&raydata.dir, vpass);

	modelList[hModel]->pfbx_->RayCast(raydata);
}
