//=========================================================
// [modelRenderer.h] 
// 作成: 2022/06/27
// 更新: 2022/07/04 実装
// 更新: 2022/12/08 インスタンシングに対応
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CModelRenderer::CModelRenderer()
	:m_modelName(std::string()), m_nVertex(0), m_pVertex(nullptr), m_nIndex(0), m_pIndex(nullptr)
{	
}

//==========================================================
// 引き数コンストラクタ
//==========================================================
CModelRenderer::CModelRenderer(std::shared_ptr<CGameObject> owner)
	:CMeshRenderer(owner), m_modelName(CHARACTER_PATH(mukade_head.obj)),
	m_pIndex(nullptr), m_pVertex(nullptr), m_nVertex(0), m_nIndex(0)
{	
}

//==========================================================
// デストラクタ
//==========================================================
CModelRenderer::~CModelRenderer()
{
	if(m_pIndex && m_pVertex)
		FinVertexArray();
	// 所有権の放棄
	if (m_pModel)
		m_pModel.reset();
}

//==========================================================
// モデル設定
//==========================================================
bool CModelRenderer::SetModel(std::string name)
{
#if _DEBUG
	auto assets = Application::Get()->GetSystem<CAssetsManager>();
	if (!assets)
		return false;
	auto modelMgr = assets->GetModelManager();
#else
	auto modelMgr = Application::Get()->GetSystem<CAssetsManager>()->GetModelManager();
#endif // _DEBUG

	// モデル取得,ポインタを受け取る
	if (auto pModel = modelMgr->GetModel(name); pModel)
	{
		m_pModel = pModel;
		modelMgr->FinishUse(name);
		m_modelName = name;

		// モデル半径の設定
		/*auto size = Transform()->GetScale();
		float scale = size.x;
		if (scale < size.y || scale < size.z)
		{
			scale = size.y;
			if (scale < size.z)
				scale = size.z;
		}*/
		//SetBSRadius(m_pModel->GetRadius() * scale);
		SetBSRadius(m_pModel->GetRadius());
		
		// 頂点配列、インデックス配列を取得しておく
		InitVertexArray();
		
		return true;
	}
	return false;
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CModelRenderer::OnLoad()
{
	CRenderer::OnLoad();
	SetModel(m_modelName);
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CModelRenderer::Awake()
{
	SetModel(m_modelName);
}

//==========================================================
// 初期化
//==========================================================
void CModelRenderer::Init()
{
	//SetModel(m_modelName);
	CMeshRenderer::Init();
}

//==========================================================
// 更新
//==========================================================
void CModelRenderer::Update()
{
}

//==========================================================
// 描画
// インスタンシング対応
// DrawSystem側から呼び出される
//==========================================================
bool CModelRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	if (!m_pModel)return false;

	auto pModelMgr = Application::Get()->GetSystem<CAssetsManager>()->GetModelManager();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();

	//--- インスタンシング描画

#if 1
	// システム側に依頼を出し、まとめて描画してもらう
	SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
	return true;

#else
	//--- インスタンシング描画
	// 自身と管理クラス以外が所持しているか確認
	if (pModelMgr->GetModelCnt(m_modelName) > 2)
	{
		// システム側に依頼を出し、まとめて描画してもらう
		SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
		return true;
	}

	//--- 不透明描画
	CLight* pLight = CLight::GetMain();
	if (!pLight)return false;

	pLight->SetEnable(GetLightEnable());	// ライティング変更
	
	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ライティング有効

	return true;
#endif // 1

}

//==========================================================
// 描画
// インスタンシング非対応
//==========================================================
bool CModelRenderer::DrawAlpha()
{
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();

	//--- 半透明部分描画
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);

	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	pDX->SetZWrite(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE)); // αブレンディング無効
	
	return true;
}

//==========================================================
// 描画
// インスタンシング非対応
//==========================================================
bool CModelRenderer::Draw(int mode)
{
	if (!m_pModel)return false;
	
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	
	//--- 不透明描画
	/*CLight* pLight = CLight::GetMain();
	if (!pLight)return false;*/
	
	m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity(mode));

	//--- 半透明部分描画
	//pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//m_pModel->Draw(pDX->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	//pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// 光源有効

	return true;
}

//==========================================================
// 頂点/インデックス配列解放
//==========================================================
void CModelRenderer::FinVertexArray()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}

//==========================================================
// 頂点情報保存
//==========================================================
void CModelRenderer::InitVertexArray()
{
	FinVertexArray();
	CAssimpModel* pModel = m_pModel.get();
	if (!pModel) return;
	pModel->GetVertexCount(&m_nVertex, &m_nIndex);
	m_pVertex = new TAssimpVertex[m_nVertex];
	m_pIndex = new UINT[m_nIndex];
	pModel->GetVertex(m_pVertex, m_pIndex);
}

//==========================================================
// レイとの当たり判定
// 衝突地点格納
//==========================================================
bool CModelRenderer::CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// 全ての三角形について繰り返し
	for (UINT i = 0; i < m_nIndex; )
	{
		// 三角形の3頂点
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2辺より法線ベクトルを求める
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// 法線ベクトルとレイの方向ベクトルとの内積(式の分母)を求める
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// 分母が0なら面と平行のため次の三角形へ
		if (base == 0.0f)
		{
			continue;
		}
		// 媒介変数tを求める
		float t;
		XMStoreFloat(&t, XMVector3Dot(n, XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// tが負なら交点はレイの後ろなので次の三角形へ
		if (t < 0.0f)
		{
			continue;
		}
		// 交点Xを求める
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// 交点が三角形の内側か調べる
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v0v1, XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v1v2, XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		// 点が内側だったので当たっている
		if (pX)
		{
			*pX = vX;
		}
		if (pN)
		{
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// 当たっていない
}

//==========================================================
// 線分との当たり判定
// 衝突地点格納
//==========================================================
bool CModelRenderer::CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX, XMFLOAT3* pN)
{
	// 全ての三角形について繰り返し
	for (UINT i = 0; i < m_nIndex; )
	{
		// 三角形の3頂点
		XMFLOAT3& vV0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3& vV2 = m_pVertex[m_pIndex[i++]].vPos;
		// 2辺より法線ベクトルを求める
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// 法線ベクトルと線分の方向ベクトルとの内積(式の分母)を求める
		XMFLOAT3 vW;
		vW.x = vP1.x - vP0.x;
		vW.y = vP1.y - vP0.y;
		vW.z = vP1.z - vP0.z;
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// 分母が0なら面と平行のため次の三角形へ
		if (base == 0.0f)
		{
			continue;
		}
		// 媒介変数tを求める
		float t;
		XMStoreFloat(&t, XMVector3Dot(n, XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// tが負なら交点は線分の後ろなので次の三角形へ
		if (t < 0.0f)
		{
			continue;
		}
		// tが1より大きい場合は線分の先なので次の三角形へ
		if (t > 1.0f)
		{
			continue;
		}
		// 交点Xを求める
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// 交点が三角形の内側か調べる
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v0v1, XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v1v2, XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		// 点が内側だったので当たっている
		if (pX)
		{
			*pX = vX;
		}
		if (pN)
		{
			XMStoreFloat3(pN, n);
		}
		return true;
	}
	return false;	// 当たっていない
}

//==========================================================
// 線分との当たり判定
// 衝突地点格納(全て)
//==========================================================
bool CModelRenderer::CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, std::vector<Vector3>* pX)
{
	Matrix4x4 mtx = Transform()->GetWorldMatrix();
	Matrix4x4 mtxPoint;
	XMFLOAT3 pos;
	// 全ての三角形について繰り返し
	for (UINT i = 0; i < m_nIndex; )
	{
		// 三角形の3頂点を現在のマトリックスから計算
		XMFLOAT3 v0 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3 v1 = m_pVertex[m_pIndex[i++]].vPos;
		XMFLOAT3 v2 = m_pVertex[m_pIndex[i++]].vPos;

		mtxPoint.Identity();
		mtxPoint._41 = v0.x;
		mtxPoint._42 = v0.y;
		mtxPoint._43 = v0.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v0 = pos;

		mtxPoint.Identity();
		mtxPoint._41 = v1.x;
		mtxPoint._42 = v1.y;
		mtxPoint._43 = v1.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v1 = pos;

		mtxPoint.Identity();
		mtxPoint._41 = v2.x;
		mtxPoint._42 = v2.y;
		mtxPoint._43 = v2.z;
		mtxPoint = mtxPoint.Multiply(mtx);
		pos = XMFLOAT3(mtxPoint._41, mtxPoint._42, mtxPoint._43);
		v2 = pos;

		XMFLOAT3& vV0 = v0;
		XMFLOAT3& vV1 = v1;
		XMFLOAT3& vV2 = v2;

		// 2辺より法線ベクトルを求める
		XMVECTOR v0v1 = XMVectorSet(vV1.x - vV0.x, vV1.y - vV0.y, vV1.z - vV0.z, 0.0f);
		XMVECTOR v1v2 = XMVectorSet(vV2.x - vV1.x, vV2.y - vV1.y, vV2.z - vV1.z, 0.0f);
		XMVECTOR n = XMVector3Normalize(XMVector3Cross(v0v1, v1v2));
		// 法線ベクトルと線分の方向ベクトルとの内積(式の分母)を求める
		XMFLOAT3 vW;
		vW.x = vP1.x - vP0.x;
		vW.y = vP1.y - vP0.y;
		vW.z = vP1.z - vP0.z;
		float base;
		XMStoreFloat(&base, XMVector3Dot(n, XMLoadFloat3(&vW)));
		// 分母が0なら面と平行のため次の三角形へ
		if (base == 0.0f)
		{
			continue;
		}
		// 媒介変数tを求める
		float t;
		XMStoreFloat(&t, XMVector3Dot(n, XMVectorSet(vV0.x - vP0.x, vV0.y - vP0.y, vV0.z - vP0.z, 0.0f)));
		t /= base;
		// tが負なら交点は線分の後ろなので次の三角形へ
		if (t < 0.0f)
		{
			continue;
		}
		// tが1より大きい場合は線分の先なので次の三角形へ
		if (t > 1.0f)
		{
			continue;
		}
		// 交点Xを求める
		XMFLOAT3 vX;
		vX.x = vP0.x + t * vW.x;
		vX.y = vP0.y + t * vW.y;
		vX.z = vP0.z + t * vW.z;
		// 交点が三角形の内側か調べる
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v0v1, XMVectorSet(vX.x - vV0.x, vX.y - vV0.y, vX.z - vV0.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v1v2, XMVectorSet(vX.x - vV1.x, vX.y - vV1.y, vX.z - vV1.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}
		XMVECTOR v2v0 = XMVectorSet(vV0.x - vV2.x, vV0.y - vV2.y, vV0.z - vV2.z, 0.0f);
		XMStoreFloat(&dot, XMVector3Dot(n,
										XMVector3Cross(v2v0, XMVectorSet(vX.x - vV2.x, vX.y - vV2.y, vX.z - vV2.z, 0.0f))));
		if (dot < 0.0f)
		{
			continue;
		}

		// 点が内側だったので当たっている
		pX->push_back(vX);
		
		/*if (pN)
		{
			XMStoreFloat3(pN, n);
		}*/
	}
	if(pX->size() != 0)
		return true;

	return false;	// 当たっていない
}


#ifdef BUILD_MODE

void CModelRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;

	//--- フォルダからファイル名取得
	if (m_aXModelList.empty() && m_aObjModelList.empty() && m_aFbxModelList.empty() || 
		ImGui::Button(u8"model reload"))
	{
		MySpace::System::CFilePath file;
		m_aXModelList = file.GetAllFileName(MODEL_PATH, ".x");
		m_aObjModelList = file.GetAllFileName(MODEL_PATH, ".obj");
		m_aFbxModelList = file.GetAllFileName(MODEL_PATH, ".fbx");
	}

	//--- モデル選択読み込み

	// xファイル
	if (auto name = DispCombo(m_aXModelList, "x File", m_modelName); !name.empty())
	{
		// ポインタを受け取る
		SetModel(name);
	}
	
	// objファイル
	if (auto name = DispCombo(m_aObjModelList, "obj File", m_modelName); !name.empty())
	{
		// ポインタを受け取る
		SetModel(name);
	}
	
	// fbxファイル
	if (auto name = DispCombo(m_aFbxModelList, "fbx File", m_modelName); !name.empty())
	{
		// ポインタを受け取る
		SetModel(name);
	}


	// 名前入力
	m_modelName = InputString(m_modelName, u8"設定モデル");
	if (ImGui::Button("Load"))
		SetModel(m_modelName);
	ImGui::Text("BSphere Radius :%f", GetBSRadius());
	ImGui::Checkbox("Static", (bool*)&m_nStaticMode);
	ImGui::SameLine();
	ImGui::Checkbox("Shadow", (bool*)&m_bShadow);

	//--- マテリアル
	ImGui::BeginTabBar("Material");
	if (ImGui::BeginTabItem("Diffuse"))
	{
		ImGui::ColorPicker4("Diffuse", (float*)&m_AssimpMaterial.Kd);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Ambient"))
	{
		ImGui::ColorPicker4("Ambient", (float*)&m_AssimpMaterial.Ka);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular"))
	{
		ImGui::ColorPicker4("Specular", (float*)&m_AssimpMaterial.Ks);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Emissive"))
	{
		ImGui::ColorPicker4("Emissive", (float*)&m_AssimpMaterial.Ke);
		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();
}

#endif // BUILD_MODE