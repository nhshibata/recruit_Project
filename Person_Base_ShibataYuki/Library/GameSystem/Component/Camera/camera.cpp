//=========================================================
// [light.cpp]
//---------------------------------------------------------
// �쐬:2022/06/27
// �X�V:2022/11/10 ������C�� : �n�����[���h�}�g���b�N�X�ԈႢ
// 
//=========================================================

//--- �C���N���[�h��
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

//--- ���O��Ԃ̐錾
using namespace MySpace::Game;
using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- ���O���
namespace Camera
{
	const float CAM_POS_P_X = 0.0f;					// �J�����̎��_�����ʒu(X���W)
	const float CAM_POS_P_Y = 30.0f;				// �J�����̎��_�����ʒu(Y���W)
	const float CAM_POS_P_Z = -100.0f;				// �J�����̎��_�����ʒu(Z���W)
	const float CAM_POS_R_X = 0.0f;					// �J�����̒����_�����ʒu(X���W)
	const float CAM_POS_R_Y = 0.0f;					// �J�����̒����_�����ʒu(Y���W)
	const float CAM_POS_R_Z = -1.0f;				// �J�����̒����_�����ʒu(Z���W)
	const float VIEW_ANGLE = 45.0f;					// ����p
//	const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// �A�X�y�N�g��
	const float VIEW_NEAR_Z = 1.0f;					// NearZ�l
	const float VIEW_FAR_Z = 10000.0f;				// FarZ�l
};
using namespace Camera;

//==========================================================
// �R���X�g���N�^
//==========================================================
CCamera::CCamera()
	:m_vTarget(1, 1, 1), m_vUp(0, 1, 0), m_vAngle(0, 0, 0), m_fFovY(0), 
	m_fAspectRatio(0), m_fFarZ(0), m_fLengthInterval(0), m_fNearZ(0),
	m_frus{ XMFLOAT4(0,0,0,0) }, m_frusw{ XMFLOAT4(0,0,0,0) }
{

}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CCamera::CCamera(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_vTarget(1,1,1),m_vUp(0,1,0),m_vAngle(0,0,0),m_fFovY(0),
	m_fAspectRatio(0), m_fFarZ(0), m_fLengthInterval(0), m_fNearZ(0),
	m_frus{ XMFLOAT4(0,0,0,0) }, m_frusw{ XMFLOAT4(0,0,0,0) }
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCamera::~CCamera()
{
	// ���g���j�����ꂽ�Ƃ���reset
	if (m_pMainCamera.lock().get() == this)
		m_pMainCamera.reset();
	else if (m_pMainCamera.lock() == GetPtr().lock())
		m_pMainCamera.reset();
	else
		return;
	
	using namespace MySpace::SceneManager;

	// �j�����ꂽ�Ƃ��ɶ�ׂ����݂��Ă��邩�m�F
	// �����Ҳݶ�ׂ��ړ�
	if (!CSceneManager::Get()->GetActiveScene())return;
	if (auto camObj = CGameObject::FindGameObjectWithTag(CDefaultTagChar::CAMERA); camObj.lock())
	{
		auto cameraCom = camObj.lock()->GetComponent<CCamera>().lock();
		SetMain(cameraCom);
	}
}

//==========================================================
// ���[�h���Ăяo��
//==========================================================
void CCamera::OnLoad()
{
	if (!m_pMainCamera.lock())
		m_pMainCamera = BaseToDerived<CCamera>();
	if(!m_pSky.lock())
		m_pSky = GetOwner()->GetComponent<CModelRenderer>();
}

//==========================================================
// �������Ăяo��
//==========================================================
void CCamera::Awake()
{
	if (!m_pMainCamera.lock())
		m_pMainCamera = BaseToDerived<CCamera>();

	// �X�J�C�h�[�����f��
	m_pSky = GetOwner()->AddComponent<CModelRenderer>();
	GetOwner()->SetLayer(CLayer::E_Layer::SKY);
	//m_pSky.lock()->SetModel(FORDER_DIR(Data/model/SkyDome/sky.fbx));
	m_pSky.lock()->SetModel(FORDER_DIR(Data/model/SkyDome/sky2.fbx));
	m_pSky.lock()->SetBSRadius(1000);
	m_pSky.lock()->SetVisible(false);
	Transform()->SetScale({ 10, 10, 10 });

	// tag�̐ݒ�
	GetOwner()->GetTagPtr()->CreateTag(CDefaultTagChar::CAMERA);
	GetOwner()->SetObjTag(CDefaultTagChar::CAMERA);

	DirectX::XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_mtxProj, XMMatrixIdentity());
}

//==========================================================
// ������
//==========================================================
void CCamera::Init()
{

	Transform()->SetPos(XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z));	// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);			// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);										// ����x�N�g��

	auto screen = CScreen::GetSize();
	m_fAspectRatio = (float)screen.x / (float)screen.y;	// �c����
	m_fFovY = VIEW_ANGLE;								// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;								// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;								// ����N���b�v����

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	auto vPos = Transform()->GetPos();
	float fVecX, fVecZ;
	fVecX = vPos.x - m_vTarget.x;
	fVecZ = vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	// ���[���h�}�g���b�N�X
	CalcWorldMatrix();

	// ���o��������
	InitFrustum();
}

//==========================================================
// �X�V
//==========================================================
void CCamera::Update()
{
	Transform()->SetRot(m_vAngle);

	// �}�g���b�N�X�X�V
	UpdateMatrix();

	// ������
	UpdateFrustum();
}

//==========================================================
// �X�J�C�h�[���`��
//==========================================================
void CCamera::DrawSkyDome()
{
	// TODO: skydome�׽���H
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
// �}�g���b�N�X
//==========================================================
void CCamera::SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	Transform()->SetPos(XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43));
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

//==========================================================
// ��ׂ̎p������}�g���b�N�X�v�Z
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
// �r���[�}�g���b�N�X
// �ÓI�����o�֐�
//=========================================================
Matrix4x4 CCamera::CalcProjMatrix(float fov, float aspect, float nearZ, float farZ)
{
	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, nearZ, farZ));
	return Matrix4x4(ret);
}

//==========================================================
// �}�g���b�N�X�X�V
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
// �����䏉����
//==========================================================
void CCamera::InitFrustum()
{
	// �����䏀��
	float fTan = tanf(XMConvertToRadians(m_fFovY * 0.5f));
	
	// �㉺���E�O��
	m_frus[0] = { 0.0f,  -1.0f, fTan,  0.0f };
	m_frus[1] = { 0.0f,  1.0f,  fTan,  0.0f };
	fTan *= m_fAspectRatio;	// �A�X�y�N�g����|����
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
// ������X�V
//==========================================================
void CCamera::UpdateFrustum()
{
	auto mtx = this->CalcWorldMatrix();	//Transform()->GetWorldMatrix();
	//--- ��ׂ̈ړ��ɔ����A����������[���h�ϊ�
	XMMATRIX mW = XMLoadFloat4x4(&mtx);
	// ���ʂ����[���h��Ԃɔz�u(XMPlaneTransform�֐��̎d�l�ɍ��킹��)
	// �t�s��
	mW = XMMatrixInverse(nullptr, mW);
	// �]�u�s��
	mW = XMMatrixTranspose(mW);

	for (int cnt = 0; cnt < MAX_FRUS; ++cnt)
	{
		XMStoreFloat4(&m_frusw[cnt], XMPlaneTransform(XMLoadFloat4(&m_frus[cnt]), mW));
	}
}

//==========================================================
// ������ɂ�铖���蔻��
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
		if (fDot < -fRadius)return EFrustumResult::OUTSIDE;	// ���S�ɊO
		if (fDot <= fRadius)bHit = true;
	}
	if (bHit)return EFrustumResult::PARTINSIDE;	// �ʂ��܂���
	return EFrustumResult::INSIDE;	// ���S�ɓ���
}

//==========================================================
// �X�N���[�����W����ܰ��ދ�ԏ�̍��W�֕ϊ�
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