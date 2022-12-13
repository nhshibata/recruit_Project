//=========================================================
// [light.cpp]
// 作成:2022/06/27
// 更新:2022/11/10 視覚錘修正 : 渡すワールドマトリックス間違い
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Manager/sceneManager.h>

#include <ImGui/imgui.h>
#include <CoreSystem/Input/input.h>
#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Render/AssimpModel.h>

//--- 名前空間の宣言
using namespace MySpace::Game;
using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- 名前空間
namespace Camera
{
	const float CAM_POS_P_X = 0.0f;					// カメラの視点初期位置(X座標)
	const float CAM_POS_P_Y = 30.0f;				// カメラの視点初期位置(Y座標)
	const float CAM_POS_P_Z = -150.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = -1.0f;					// カメラの注視点初期位置(Z座標)
	const float VIEW_ANGLE = 45.0f;					// 視野角
//	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
	const float VIEW_NEAR_Z = 1.0f;					// NearZ値
	const float VIEW_FAR_Z = 10000.0f;				// FarZ値
};
using namespace Camera;

CCamera::CCamera()
{
}
CCamera::CCamera(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_vPos(0,0,0),m_vTarget(1,1,1),m_vUp(0,1,0),m_vAngle(0,0,0)
{
	//if (!m_pCamera)m_pCamera = this;
	//if (!m_pCamera.lock())m_pCamera = BaseToDerived<CCamera>();
}
CCamera::~CCamera()
{
	// 自身が破棄されたときにreset
	if (m_pMainCamera.lock().get() == this)
		m_pMainCamera.reset();
	else if (m_pMainCamera.lock() == GetPtr().lock())
		m_pMainCamera.reset();
	else
		return;
	
	using namespace MySpace::SceneManager;

	// 破棄されたときにｶﾒﾗが存在しているか確認
	// あればﾒｲﾝｶﾒﾗを移動
	if (!CSceneManager::Get().GetActiveScene())return;
	if (auto camObj = CGameObject::FindGameObjectWithTag(CDefaultTagChar::CAMERA); camObj.lock())
	{
		auto cameraCom = camObj.lock()->GetComponent<CCamera>().lock();
		SetMain(cameraCom);
	}
}
void CCamera::OnLoad()
{
	if (!m_pMainCamera.lock())
		m_pMainCamera = BaseToDerived<CCamera>();

	m_pSky = GetOwner()->GetComponent<CModelRenderer>();
}
void CCamera::Awake()
{
	if (!m_pMainCamera.lock())
		m_pMainCamera = BaseToDerived<CCamera>();

	// スカイドームモデル
	m_pSky = GetOwner()->AddComponent<CModelRenderer>();
	GetOwner()->SetLayer(CLayer::E_Layer::SKY);
	//m_pSky.lock()->SetModel(FORDER_DIR(Data/model/SkyDome/sky.fbx));
	m_pSky.lock()->SetModel(FORDER_DIR(Data/model/SkyDome/sky2.fbx));
	m_pSky.lock()->SetBSRadius(1000);
	m_pSky.lock()->SetVisible(false);
	Transform()->SetScale({ 10, 10, 10 });

	// tagの設定
	GetOwner()->GetTagPtr()->CreateTag(CDefaultTagChar::CAMERA);
	GetOwner()->SetTag(CDefaultTagChar::CAMERA);

	DirectX::XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_mtxProj, XMMatrixIdentity());
}
void CCamera::Init()
{
	auto screen = CScreen::GetSize();

	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);							// 上方ベクトル

	m_fAspectRatio = (float)screen.x / (float)screen.y;	// 縦横比
	m_fFovY = VIEW_ANGLE;								// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;								// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;								// 後方クリップ距離

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float fVecX, fVecZ;
	fVecX = m_vPos.x - m_vTarget.x;
	fVecZ = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	// ワールドマトリックス
	CalcWorldMatrix();

	// 視覚錘初期化
	InitFrustum();
}
void CCamera::Update()
{
	m_vPos.x = Transform()->GetPos().x;
	m_vPos.y = Transform()->GetPos().y;
	m_vPos.z = Transform()->GetPos().z;

	// マトリックス更新
	UpdateMatrix();

	// 視錘台
	UpdateFrustum();
}
void CCamera::DrawSkyDome()
{
	//float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	//ID3D11DeviceContext* pDC = CDXDevice::Get().GetDeviceContext();
	//pDC->ClearRenderTargetView(CDXDevice::Get().GetRenderTargetView(), ClearColor);
	//pDC->ClearDepthStencilView(CDXDevice::Get().GetDepthStencilView(),
	//	D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//ID3D11RenderTargetView* pViews[] = {
	//	CDXDevice::Get().GetRenderTargetView()
	//};
	//pDC->OMSetRenderTargets(1, pViews,nullptr);

	// TODO: skydomeｸﾗｽ作る？
	if (!m_pSky.lock())
		return;
	//{
	//	CDXDevice::Get().SetZBuffer(false);		// Zバッファ無効
	//	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::Get());
	//	pLight->SetDisable();	// ライティング無効

	//	XMFLOAT4X4 mW, oldW;
	//	oldW = Transform()->GetWorldMatrix();
	//	XMStoreFloat4x4(&mW, XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z));
	//	Transform()->SetWorldMatrix(mW);

	//	m_pSky.lock()->Draw(0);
	//	Transform()->SetWorldMatrix(oldW);
	//	pLight->SetEnable();	// ライティング有効
	//}
	//CDXDevice::Get().SetZBuffer(true);
	//CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	m_pSky.lock()->SetVisible(true);
	m_pSky.lock()->Draw(0);
	m_pSky.lock()->SetVisible(false);

}
void CCamera::SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}
DirectX::XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos),
		XMLoadFloat3(&m_vTarget),
		XMLoadFloat3(&m_vUp)
		));

	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), 
		m_fAspectRatio, m_fNearZ, m_fFarZ
	));
}
void CCamera::InitFrustum()
{
	// 視錘台準備
	float fTan = tanf(XMConvertToRadians(m_fFovY * 0.5f));
	
	// 上下左右前後
	m_frus[0] = { 0.0f, -1.0f, fTan, 0.0f };
	m_frus[1] = { 0.0f, 1.0f, fTan, 0.0f };
	fTan *= m_fAspectRatio;	// アスペクト比を掛ける
	m_frus[2] = { 1.0f, 0.0f, fTan, 0.0f };
	m_frus[3] = { -1.0f, 0.0f, fTan, 0.0f };
	m_frus[4] = { 0.0f, 0.0f,1.0f, -m_fNearZ };
	m_frus[5] = { 0.0f, 0.0f,-1.0f, m_fFarZ };

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		XMStoreFloat4(&m_frus[cnt], XMPlaneNormalize(XMLoadFloat4(&m_frus[cnt])));
	}

	if (Transform())
	{
		Transform()->SetPos(m_vPos);
	}
}
void CCamera::UpdateFrustum()
{
	auto mtx = this->CalcWorldMatrix();	//Transform()->GetWorldMatrix();
	// ｶﾒﾗの移動に伴い、視錘台をワールド変換
	XMMATRIX mW = XMLoadFloat4x4(&mtx);
	// 平面をワールド空間に配置
	mW = XMMatrixInverse(nullptr, mW);
	mW = XMMatrixTranspose(mW);

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		XMStoreFloat4(&m_frusw[cnt], XMPlaneTransform(XMLoadFloat4(&m_frus[cnt]), mW));
	}
}
// *@視錘台
CCamera::EFrustumResult CCamera::CollisionViewFrustum(XMFLOAT3* pCenter, float fRadius)
{
	bool bHit = false;
	XMVECTOR frusw, center, dot;
	float fDot;
	center = XMLoadFloat3(pCenter);

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		frusw = XMLoadFloat4(&m_frusw[cnt]);
		dot = XMPlaneDotCoord(frusw, center);
		XMStoreFloat(&fDot, dot);
		if (fDot < -fRadius)return EFrustumResult::OUTSIDE;	// 完全に外
		if (fDot <= fRadius)bHit = true;
	}
	if (bHit)return EFrustumResult::PARTINSIDE;	// 面をまたぐ
	return EFrustumResult::INSIDE;	// 完全に内側
}

#ifdef BUILD_MODE

void CCamera::ImGuiDebug()
{
	
}

#endif // BUILD_MODE