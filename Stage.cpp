#include "Stage.h"
#include "resource1.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include <iostream>
#include <string>


//ブロック設置
void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type = _type;
}

//ブロックの高さ
void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
}

//コンストラクタ
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_{ -1, -1, -1, -1, -1 }
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		hModel_[i] = -1;
	}
		for (int z = 0; z < XSIZE; z++)
		{
			for (int x = 0; x < XSIZE; x++)
				SetBlock(x, z, DEFAULT);
		}
	
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
	string modelName[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx",
		"BoxGrass.fbx"
	};
	string filename_base = "assets/";

    //モデルデータのロード
	for (int i = 0; i < 5; i++) 
	{
		hModel_[i] = Model::Load(filename_base+modelName[i]);
		assert(hModel_[i] >= 0);
	}
	for (int z = 0; z < ZSIZE; z++)
	{
		for (int x = 0; x < XSIZE; x++)
		{
			SetBlock(x,z,(BLOCKTYPE)(0));
			SetBlockHeight(x, z,1);
		}
	}
}

//更新
void Stage::Update()
{
	if (!Input::IsMouseButton(0)) {
		return;
	}
	float w = (float)(Direct3D::scrWidth / 2.0f);
	float h = (float)(Direct3D::scrHeight / 2.0f);

	//Offsetx,y は0
	//minZ =0 maxZ = 1

	XMMATRIX vp =
	{
		 w,  0,  0, 0,
		 0, -h,  0, 0,
		 0,  0,  1, 0,
		 w,  h,  0, 1
	};

	//ビューポート
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//プロジェクション変換
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//ビュー変換
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;
	//①　mousePosFrontをベクトルに変換
	XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
	//②　①にinvVP、invPrj、invViewを掛ける
	vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
	//③　mousePosBackをベクトルに変換
	XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
	//④　③にinvVP、invPrj、invViewを掛ける
	vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

	int bufX = -1, bufZ;
	float minDistance = 9999;

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//⑤　②から④に向かってレイを打つ（とりあえずモデル番号はhModel_[0]）
				RayCastData data;
				XMStoreFloat4(&data.start, vMouseFront);
				XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);

				//⑥　レイが当たったらブレイクポイントで止める
				if (data.hit)
				{
					if (minDistance > data.dist)
					{
						minDistance = data.dist;
						bufX = x;
						bufZ = z;
						nowBrockX = x;
						nowBrockZ = z;
					}
				}

			}
		}
	}
	if (bufX >= 0)
	{
		switch (mode_)
		{
		case 0:
			if ((nowBrockX == curBrockX) && (nowBrockZ == curBrockZ)) {
				if (Input::IsMouseButtonDown(0))
				{
					table_[bufX][bufZ].height++;
				}
				break;
			}

			else {
				curBrockX = nowBrockX;
				curBrockZ = nowBrockZ;
				table_[bufX][bufZ].height++;
			}
			break;

		case 1:
			if ((nowBrockX == curBrockX) && (nowBrockZ == curBrockZ)) {
				if (Input::IsMouseButtonDown(0) && (table_[bufX][bufZ].height > 1))
				{
					table_[bufX][bufZ].height--;
				}
				break;
			}

			else {
				curBrockX = nowBrockX;
				curBrockZ = nowBrockZ;
				if (table_[bufX][bufZ].height > 1)
					table_[bufX][bufZ].height--;
			}
			break;

		case 2:
			if ((nowBrockX == curBrockX) && (nowBrockZ == curBrockZ)) {
				if (Input::IsMouseButtonDown(0))
				{
					table_[bufX][bufZ].type = select_;
				}
				break;
			}

			else {
				curBrockX = nowBrockX;
				curBrockZ = nowBrockZ;
				table_[bufX][bufZ].type = select_;
			}
			break;

		case 3:
				int savetype = table_[bufX][bufZ].type;
				for (int x = 0; x < 15; x++)
				{
					for (int z = 0; z < 15; z++)
					{
						if ((savetype == table_[x][z].type) && (table_[bufX][bufZ].height == table_[x][z].height))
							table_[x][z].type = select_;
					}

			
			}break;
		}
		
	}

}

//描画
void Stage::Draw()
{
	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 0; y++)
			{
				//table[x][z]からオブジェクトのタイプを取り出して書く！
				int type = table_[x][z].type;
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[type], trans);
				Model::Draw(hModel_[type]);
			}
		}
	}
}

//開放
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) 
{
	switch (msg)
	{
		//ダイアログできた
	case WM_INITDIALOG:
		//ラジオボタンの初期値
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);

		//コンボボックスの初期値(できてない)
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"草原");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"砂地");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"水");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0, 0);

		//やりたいこと”ラジオボタンにチェックを入れたらモードを0，1にする”
		// ０の時は盛り上げる、１の時は盛り下げる
		// ...ができたらよかった。
		//SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BST_CHECKED, 0, 0);
		//SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BST_CHECKED, 0, 0);

		///SendMessage(GetDlgItem(hDlg, IDC_MENU_SAVE),MN_SET)

	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDC_RADIO_UP:
			mode_ = 0;
			return TRUE;

		case IDC_RADIO_DOWN:
			mode_ = 1;
			return TRUE;

		case IDC_RADIO_CHANGE:
			mode_ = 2;
			return TRUE;

		case IDC_COMBO1:
			select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
			return	TRUE;

		case IDC_RADIO_COLOR:
			mode_ = 3;
			return TRUE;
		}




		return TRUE;
	}
	return FALSE;
}

void Stage::Save()
{
	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //ファイルの種類
		TEXT("すべてのファイル(*.*)\0*.*\0\0");
	ofn.lpstrFile = fileName;               	            //ファイル名
	ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		                //フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return;



	HANDLE hFile;        //ファイルのハンドル
	hFile = CreateFile(
		fileName,               //ファイル名
		GENERIC_WRITE,          //アクセスモード（書き込み用）
		0,                      //共有（なし）
		NULL,                   //セキュリティ属性（継承しない）
		CREATE_ALWAYS,          //作成方法
		FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
		NULL);                  //拡張属性（なし）
	string mapData;
	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			string mapHeight = std::to_string(table_[x][z].height);
			string mapType = std::to_string(table_[x][z].type); // ブロックの種類を数値に変換
			mapData += mapHeight + "," + mapType; // 高さと種類をカンマ区切りで追加
			if (x != 14 || z != 14) {  // マップの最後でない場合、カンマを追加
				mapData += ",";
			}
		}
	}
	const char* cstr = mapData.c_str();


	DWORD dwBytes = 0;  //書き込み位置
	WriteFile(
		hFile,              //ファイルハンドル
		cstr,                  //保存するデータ（文字列）
		(DWORD)strlen(cstr),   //書き込む文字数
		&dwBytes,           //書き込んだサイズを入れる変数
		NULL);              //オーバーラップド構造体（今回は使わない）

	CloseHandle(hFile);
}

void Stage::Load()
{
	char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	            //名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	            //構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);             	//構造体のサイズ
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")  //ファイルの種類
		TEXT("すべてのファイル(*.*)\0*.*\0\0");             
	ofn.lpstrFile = fileName;               	            //ファイル名
	ofn.nMaxFile = MAX_PATH;               	                //パスの最大文字数
	ofn.Flags = OFN_FILEMUSTEXIST;   		                //フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	            //デフォルト拡張子

	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetOpenFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return;

	HANDLE hFile;        //ファイルのハンドル
	hFile = CreateFile(
		fileName,                 //ファイル名
		GENERIC_READ,           //アクセスモード（書き込み用）
		0,                      //共有（なし）
		NULL,                   //セキュリティ属性（継承しない）
		OPEN_EXISTING,           //作成方法
		FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
		NULL);                  //拡張属性（なし）


	//ファイルのサイズを取得
	DWORD fileSize = GetFileSize(hFile, NULL);

	//ファイルのサイズ分メモリを確保
	char* data;
	data = new char[fileSize];

	DWORD dwBytes = 0; //読み込み位置

	ReadFile(
		hFile,     //ファイルハンドル
		data,      //データを入れる変数
		fileSize,  //読み込むサイズ
		&dwBytes,  //読み込んだサイズ
		NULL);     //オーバーラップド構造体（今回は使わない）

	char* nextToken;
	char* token = strtok_s(data, ",", &nextToken); // カンマで文字列を分割

	int count = 0;
	while (token != NULL)
	{
		if (count >= 15 * 15) {
			break;  // データが多すぎる場合は終了
		}

		int height_ = atoi(token);
		token = strtok_s(NULL, ",", &nextToken);

		if (token != NULL) {
			int type_ = atoi(token); // ブロックの種類の数値を読み込む
			table_[count / 15][count % 15].height = height_;
			table_[count / 15][count % 15].type = static_cast<BLOCKTYPE>(type_); // 数値からブロックの種類に変換
		}

		token = strtok_s(NULL, ",", &nextToken); // 次のトークンを取得
		count++;
	}	CloseHandle(hFile);
}