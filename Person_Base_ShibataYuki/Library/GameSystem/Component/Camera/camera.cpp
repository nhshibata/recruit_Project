//=========================================================
// [light.cpp]
//---------------------------------------------------------
// 作成:2022/06/27
// 更新:2022/11/10 視錘台修正 : 渡すワールドマトリックス間違い
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
	const float CAM_POS_P_Z = -100.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = -1.0f;				// カメラの注視点初期位置(Z座標)
	const float VIEW_ANGLE = 45.0f;					// 視野角
//	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
	const float VIEW_NEAR_Z = 1.0f;					// NearZ値
	const float VIEW_FAR_Z = 10000.0f;				// FarZ値
};
using namespace Camera;

//==========================================================
// コンストラクタ
//==========================================================
CCamera::CCamera()
	:m_vTarget(1, 1, 1), m_vUp(0, 1, 0), m_vAngle(0, 0, 0), m_fFovY(0), 
	m_fAspectRatio(0), m_fFarZ(0), m_fLengthInterval(0), m_fNearZ(0),
	m_frus{ XMFLOAT4(0,0,0,0) }, m_frusw{ XMFLOAT4(0,0,0,0) }
{

}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CCamera::CCamera(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_vTarget(1,1,1),m_vUp(0,1,0),m_vAngle(0,0,0),m_fFovY(0),
	m_fAspectRatio(0), m_fFarZ(0), m_fLengthInterval(0), m_fNearZ(0),
	m_frus{ XMFLOAT4(0,0,0,0) }, m_frusw{ XMFLOAT4(0,0,0,0) }
{
}

//==========================================================
// デストラクタ
//==========================================================
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
	if (!CSceneManager::Get()->GetActiveScene())return;
	if (auto camObj = CGameObject::FindGameObjectWithTag(CDefaultTagChar::CAMERA); camObj.lock())
	{
		auto cameraCom = camObj.lock()->GetComponent<CCamera>().lock();
		SetMain(cameraCom);
	}
}

//==========================================================
// ロード時呼び出し
//==========================================================
void CCamera::OnLoad()
{
	if (!m_pMainCamera.lock())
		m_pMainCamera = BaseToDerived<CCamera>();
	if(!m_pSky.lock())
		m_pSky = GetOwner()->GetComponent<CModelRenderer>();
}

//==========================================================
// 生成時呼び出し
//==========================================================
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
	GetOwner()->SetObjTag(CDefaultTagChar::CAMERA);

	DirectX::XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_mtxProj, XMMatrixIdentity());
}

//==========================================================
// 初期化
//==========================================================
void CCamera::Init()
{

	Transform()->SetPos(XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z));	// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);			// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);										// 上方ベクトル

	auto screen = CScreen::GetSize();
	m_fAspectRatio = (float)screen.x / (float)screen.y;	// 縦横比
	m_fFovY = VIEW_ANGLE;								// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;								// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;								// 後方クリップ距離

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	auto vPos = Transform()->GetPos();
	float fVecX, fVecZ;
	fVecX = vPos.x - m_vTarget.x;
	fVecZ = vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	// ワールドマトリックス
	CalcWorldMatrix();

	// 視覚錘初期化
	InitFrustum();
}

//==========================================================
// 更新
//==========================================================
void CCamera::Update()
{
	Transform()->SetRot(m_vAngle);

	// マトリックス更新
	UpdateMatrix();

	// 視錘台
	UpdateFrustum();
}

//==========================================================
// スカイドーム描画
//==========================================================
void CCamera::DrawSkyDome()
{
	// TODO: skydomeｸﾗｽ作る？
	if (!m_pSky.lock())
		return;

	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pLight = CLight::GetMain();

	pLight->SetDisable();
	pDX->SetBlendState((int)EBlendState::BS_ALPHABLEND);
	pDX->SetZBuffer(false);
	
	m_pSky.lock()->SetVisible(true);
	m_pSky.lock()->Draw(EByOpacity::eOpacityOnly);
	m_pSky.lock()->SetVisible(false);

	pDX->SetZBuffer(true);
	pDX->SetBlendState((int)EBlendState::BS_NONE);
	pLight->SetEnable();
}

//==========================================================
// マトリックス
//==========================================================
void CCamera::SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	Transform()->SetPos(XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43));
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

//==========================================================
// ｶﾒﾗの姿勢からマトリックス計算
//==========================================================
Matrix4x4& CCamera::CalcWorldMatrix()
{
	auto vPos = Transform()->GetPos();
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - vPos.x, m_vTarget.y - vPos.y, m_vTarget.z - vPos.z, 0.0f);
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
	m_mtxWorld._41 = vPos.x;
	m_mtxWorld._42 = vPos.y;
	m_mtxWorld._43 = vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}

//=========================================================
// ビューマトリックス
// 静的メンバ関数
//=========================================================
Matrix4x4 CCamera::CalcProjMatrix(float fov, float aspect, float nearZ, float farZ)
{
	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, nearZ, farZ));
	return Matrix4x4(ret);
}

//==========================================================
// マトリックス更新
//==========================================================
void CCamera::UpdateMatrix()
{
	auto vPos = Transform()->GetPos();
	XMStoreFloat4x4(&m_mtxView, 
					XMMatrixLookAtLH(XMLoadFloat3(&vPos),XMLoadFloat3(&m_vTarget),XMLoadFloat3(&m_vUp))
	);

	/*XMStoreFloat4x4(&m_mtxProj, 
					XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ)	
	);*/
	m_mtxProj = CalcProjMatrix(m_fFovY, m_fAspectRatio, m_fNearZ, m_fFarZ);
}

//==========================================================
// 視錘台初期化
//==========================================================
void CCamera::InitFrustum()
{
	// 視錘台準備
	float fTan = tanf(XMConvertToRadians(m_fFovY * 0.5f));
	
	// 上下左右前後
	m_frus[0] = { 0.0f,  -1.0f, fTan,  0.0f };
	m_frus[1] = { 0.0f,  1.0f,  fTan,  0.0f };
	fTan *= m_fAspectRatio;	// アスペクト比を掛ける
	m_frus[2] = { 1.0f,  0.0f,  fTan,  0.0f };
	m_frus[3] = { -1.0f, 0.0f,  fTan,  0.0f };
	m_frus[4] = { 0.0f,  0.0f,  1.0f,  -m_fNearZ };
	m_frus[5] = { 0.0f,  0.0f,  -1.0f, m_fFarZ };

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		XMStoreFloat4(&m_frus[cnt], XMPlaneNormalize(XMLoadFloat4(&m_frus[cnt])));
	}

}

//==========================================================
// 視錘台更新
//==========================================================
void CCamera::UpdateFrustum()
{
	auto mtx = this->CalcWorldMatrix();	//Transform()->GetWorldMatrix();
	//--- ｶﾒﾗの移動に伴い、視錘台をワールド変換
	XMMATRIX mW = XMLoadFloat4x4(&mtx);
	// 平面をワールド空間に配置(XMPlaneTransform関数の仕様に合わせる)
	// 逆行列
	mW = XMMatrixInverse(nullptr, mW);
	// 転置行列
	mW = XMMatrixTranspose(mW);

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		XMStoreFloat4(&m_frusw[cnt], XMPlaneTransform(XMLoadFloat4(&m_frus[cnt]), mW));
	}
}

//==========================================================
// 視錘台による当たり判定
//==========================================================
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

//==========================================================
// スクリーン座標からﾜｰﾙﾄﾞ空間上の座標へ変換
//==========================================================
Vector3 CCamera::ConvertScreenToWorld(Vector2 pos)
{
	D3D11_VIEWPORT& vp = *Application::Get()->GetSystem<CDXDevice>()->GetViewPort();
	Vector3 ret;
	XMStoreFloat3(&ret, XMVector3Unproject(
		XMVectorSet(pos.x, pos.y, 0.0f, 1.0f),
		vp.TopLeftX,
		vp.TopLeftY,
		vp.Width,
		vp.Height,
		vp.MinDepth,
		vp.MaxDepth,
		XMLoadFloat4x4(&GetProjMatrix()),
		XMLoadFloat4x4(&GetViewMatrix()),
		XMMatrixIdentity()
	));
	return ret;
}


#ifdef BUILD_MODE

void CCamera::ImGuiDebug()
{
	
}

#endif // BUILD_MODE