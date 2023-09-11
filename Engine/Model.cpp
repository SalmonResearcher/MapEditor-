#include "Model.h"
#include "Direct3D.h"

namespace Model {

	//モデルのポインタをぶち込んでおくベクタ
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string fileName)
{
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = fileName;
	pData->pfbx_ = nullptr;

	//ファイルネームが同じだったら、読まん！
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
	//モデル番号は、modelListのインデックス
}
void Model::Draw(int hModel) {
	//モデル番号は、modelListのインデックス
	modelList[hModel]->pfbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false; //参照されてる？
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
	//モデルのトランスフォームをカリキュレーション
	modelList[hModel]->transform_.Calclation();
	//①ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr,
					modelList[hModel]->transform_.GetWorldMatrix());
	//②レイの通過点を求める（モデル空間でのレイの方向ベクトルを求める）
	XMVECTOR vpass{ raydata.start.x + raydata.dir.x,
					raydata.start.y + raydata.dir.y,
					raydata.start.z + raydata.dir.z,
					raydata.start.w + raydata.dir.w };
	//③rayData.statをモデル空間に変換（①を掛ける）
	XMVECTOR vstart = XMLoadFloat4(&raydata.start);
	vstart = XMVector3TransformCoord(vstart, wInv); //tarnsformcoordはw要素を無視してくれるらしい
	XMStoreFloat4(&raydata.start, vstart); //これ　値をセットすんの忘れてた。。。

	//④（始点から方向ベクトルをちょい伸ばした先）通過点（②）に①を掛ける
	vpass = 
	//⑤rayData.dirを③から④に向かうベクトルにする（引き算）
	vpass = vpass - vstart;
	XMStoreFloat4(&raydata.dir, vpass);

	modelList[hModel]->pfbx_->RayCast(raydata);
}
