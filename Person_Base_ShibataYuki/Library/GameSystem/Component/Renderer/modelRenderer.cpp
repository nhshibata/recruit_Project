//=========================================================
// [modelRenderer.h] 
// 作成: 2022/06/27
// 更新: 2022/07/04 実装
// 更新: 2022/12/08 インスタンシングに対応
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

CModelRenderer::CModelRenderer(std::shared_ptr<CGameObject> owner)
	:CMeshRenderer(owner), m_modelName(CHARACTER_PATH(mukade_head.obj)),
	m_pIndex(nullptr), m_pVertex(nullptr), m_nVertex(0), m_nIndex(0),
	m_bInstancing(false)
{
	
}
CModelRenderer::~CModelRenderer()
{
	if(m_pIndex && m_pVertex)
		FinVertexArray();
	if (m_pModel)
	{
		// 所有権の放棄
		m_pModel.reset();
	}
}
void CModelRenderer::SetModel(std::string name)
{
	// ポインタを受け取る
	if (m_pModel = CModelManager::Get()->GetModel(name); m_pModel)
	{
		m_modelName = name;

		// モデル半径の設定
		auto size = Transform()->GetScale();
		float scale = size.x;
		if (scale < size.y || scale < size.z)
		{
			scale = size.y;
			if (scale < size.z)
				scale = size.z;
		}
		SetBSRadius(m_pModel->GetRadius() * scale);

		// 自身と管理ｸﾗｽ以外に所有者が居た時
		if (int num = CModelManager::Get()->GetModelCnt(name); num > 2)
		{
			m_bInstancing = true;
		}
	}

	// 頂点配列、インデックス配列を取得しておく
	InitVertexArray();
}
void CModelRenderer::Awake()
{
	GetOwner()->SetLayer(CLayer::E_Layer::MODEL);
	SetModel(m_modelName);
}
void CModelRenderer::Init()
{
	//SetModel(m_modelName);
	CMeshRenderer::Init();
}
void CModelRenderer::Update()
{
}
bool CModelRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	if (!m_pModel)return false;

	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	//--- インスタンシング描画
	// 自身と管理以外が所持しているか確認
	if (m_bInstancing || CModelManager::Get()->GetModelCnt(m_modelName) > 2)
	{
		// システム側に依頼を出し、まとめて描画してもらう
		SceneManager::CSceneManager::Get()->GetDrawSystem()->SetInstanchingModel(m_modelName, mtx);
		return true;
	}

	//--- 不透明描画
	CLight* pLight = CLight::Get();
	if (!pLight)return false;
	pLight->SetDisable(GetLightEnable());	// ライティング無効
	
	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ライティング有効

	//--- 半透明部分描画
	CDXDevice::Get()->SetZBuffer(false);
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	CDXDevice::Get()->SetZBuffer(true);			// αブレンディング無効
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// 光源有効

	return true;
}
bool CModelRenderer::Draw(int no)
{
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	
	if (!m_pModel)return false;

	//--- 不透明描画
	CLight* pLight = CLight::Get();
	if (!pLight)return false;
	pLight->SetDisable(GetLightEnable());	// ライティング無効

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ライティング有効

	//--- 半透明部分描画
	CDXDevice::Get()->SetZBuffer(false);
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	m_pModel->Draw(CDXDevice::Get()->GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	CDXDevice::Get()->SetZBuffer(true);			// αブレンディング無効
	CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// 光源有効

	return true;
}
// 頂点/インデックス配列解放
void CModelRenderer::FinVertexArray()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}
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
// レイとの当たり判定
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

// 線分との当たり判定
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

#ifdef BUILD_MODE

void CModelRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;

	CMeshRenderer::ImGuiDebug();

	if (m_aXModelList.empty() && m_aObjModelList.empty() && m_aFbxModelList.empty() || ImGui::Button("モデル reload"))
	{
		MySpace::System::CFilePath file;
		m_aXModelList = file.GetAllFileName(MODEL_PATH, ".x");
		m_aObjModelList = file.GetAllFileName(MODEL_PATH, ".obj");
		m_aFbxModelList = file.GetAllFileName(MODEL_PATH, ".fbx");
	}

	// 名前入力
	m_modelName = InputString(m_modelName, u8"設定モデル");

	// モデル
	if (auto name = DispMenuBar(m_aXModelList, "xFile"); !name.empty())
	{
		m_modelName = name;

		// ポインタを受け取る
		m_pModel = CModelManager::Get()->GetModel(name);
	}
	
	if (auto name = DispMenuBar(m_aObjModelList, "objFile"); !name.empty())
	{
		m_modelName = name;

		// ポインタを受け取る
		m_pModel = CModelManager::Get()->GetModel(name);
	}
	
	if (auto name = DispMenuBar(m_aFbxModelList, "fbxFile"); !name.empty())
	{
		m_modelName = name;

		// ポインタを受け取る
		m_pModel = CModelManager::Get()->GetModel(name);
		SetModel(name);
	}
}

#endif // BUILD_MODE