//=========================================================
// [myCamera.h] 
// 作成:2022/08/18
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <myCamera.h>
#include <centipede.h>
#include <CoreSystem/Input/input.h>

namespace {

	const float TP_POS_P_X = 0.0f;		// 三人称カメラの視点位置(X座標)
	const float TP_POS_P_Y = 100.0f;	// 三人称カメラの視点位置(Y座標)
	const float TP_POS_P_Z = 0.0f;		// 三人称カメラの視点位置(Z座標)
	const float TP_POS_R_X = 0.0f;		// 三人称カメラの注視点位置(X座標)
	const float TP_POS_R_Y = -50.0f;		// 三人称カメラの注視点位置(Y座標)
	const float TP_POS_R_Z = 0.0f;		// 三人称カメラの注視点位置(Z座標)

}

CMyCamera::CMyCamera(std::shared_ptr<CGameObject> owner)
	:CCamera(owner), m_rotation(0), m_rotationValue(XM_PI * 0.01f), m_distance(20)
{

}
void CMyCamera::Awake()
{
	m_fOffset = 400.0f;
	CCamera::Awake();
	GetOwner()->SetName("MyCamera");

	SetMain(BaseToDerived<CCamera>());
}
void CMyCamera::Init()
{
	CCamera::Init();
	if(auto obj = CGameObject::FindGameObjectWithTag("Player").lock();obj)
		m_pTarget = obj->GetComponent<CCentipede>();
}
void CMyCamera::Update()
{
	if (!m_pTarget.lock())
	{
		CCamera::Update();
		return;
	}

	auto com = m_pTarget.lock()->Transform();
	auto targetPos = com->GetPos();
	auto pos = Transform()->GetPos();
	auto target = GetTarget();

	// xz座標を合わせる
	//pos.x = targetPos.x;
	//pos = Vector3(targetPos.x, targetPos.y + 150, targetPos.z);
	//pos.z = com->GetPos().z;
	
	//target = targetPos;

	//XMMATRIX mW;
	//XMFLOAT3 vNewUp, vNewTarget, vNewPos;
	//mW = XMLoadFloat4x4(&m_pTarget.lock()->Transform()->GetWorldMatrix());
	//XMStoreFloat3(&vNewUp,
	//	XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
	//XMStoreFloat3(&vNewTarget,
	//	XMVector3TransformCoord(
	//		XMVectorSet(TP_POS_R_X, TP_POS_R_Y, TP_POS_R_Z, 1.0f), mW));
	//XMStoreFloat3(&vNewPos,
	//	XMVector3TransformCoord(
	//		XMVectorSet(TP_POS_P_X, TP_POS_P_Y, TP_POS_P_Z, 1.0f), mW));
	//m_vUp.x = m_vUp.x * 0.9f + vNewUp.x * 0.1f;
	//m_vUp.y = m_vUp.y * 0.9f + vNewUp.y * 0.1f;
	//m_vUp.z = m_vUp.z * 0.9f + vNewUp.z * 0.1f;
	////target.x = target.x * 0.9f + vNewTarget.x * 0.1f;
	////target.y = target.y * 0.9f + vNewTarget.y * 0.1f;
	////target.z = target.z * 0.9f + vNewTarget.z * 0.1f;
	////pos.x = pos.x * 0.9f + vNewPos.x * 0.1f;
	////pos.y = pos.y * 0.9f + vNewPos.y * 0.1f;
	////pos.z = pos.z * 0.9f + vNewPos.z * 0.1f;
	//target = Vector3(target.x * 0.9f + vNewTarget.x * 0.1f, target.y * 0.9f + vNewTarget.y * 0.1f,
	//	target.z * 0.9f + vNewTarget.z * 0.1f);
	//pos = Vector3(pos.x * 0.9f + vNewPos.x * 0.1f, pos.y * 0.9f + vNewPos.y * 0.1f,
	//	pos.z * 0.9f + vNewPos.z * 0.1f);

	//if (CInput::GetKeyPress(VK_J))
	//{
	//	m_rotation -= m_rotationValue;
	//	if (m_rotation < 0)
	//	{
	//		m_rotation += XM_PI * 2.0f;
	//	}
	//}
	//if (CInput::GetKeyPress(VK_L))
	//{
	//	m_rotation += m_rotationValue;
	//	if (m_rotation > XM_PI * 2.0f)
	//	{
	//		m_rotation -= XM_PI * 2.0f;
	//	}
	//}
	// 成長率に合わせてｶﾒﾗ座標を上に移動する
	static float offset = 0;
	if (CInput::GetKeyPress(VK_I))
		offset += 0.5f;
	if (CInput::GetKeyPress(VK_O))
		offset -= 0.5f;

	float x = targetPos.x + sinf(m_rotation) * m_distance;
	float y = targetPos.y + m_fOffset;
	float z = targetPos.z + cosf(m_rotation) * m_distance;
	pos = Vector3(x, y, z);

	targetPos.y += offset;

	Transform()->SetPos(pos);
	m_vPos = pos;
	SetTarget(targetPos);

	CCamera::Update();
}