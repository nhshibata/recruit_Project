//=========================================================
// [rigidbody.cpp]
//---------------------------------------------------------
// �쐬:2022/05/24
// �X�V:2023/01/31 �ُ�ȍX�V���N���Ă������߁A����
//				FixedUpdate�̌Ăяo�������̏C���~�X
//---------------------------------------------------------
// ����
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//--- �萔��`
namespace 
{
	const float GRAVITY = -9.8f;		// �d�� ���W�n�ɂ���Č������Ⴄ
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CRigidbody::CRigidbody()
{
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CRigidbody::CRigidbody(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
	, m_bGravity(true), m_bIsSleep(false), m_fGravity(GRAVITY), m_fResistance(1.0f), m_fMass(1.0f),
	m_vAccel(0, 0, 0), m_vTargetPos(1, 1, 1), m_vVel(0, 0, 0), m_vForce(0, 0, 0)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CRigidbody::~CRigidbody()
{

}

//==========================================================
// �X�V
//==========================================================
void CRigidbody::FixedUpdate()
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();
	Vector3 oldPos = GetOwner()->GetTransform()->GetOldPos();
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	{
		// �d�͂�^����
		if (m_bGravity)
		{
			m_vForce.y += float(m_fGravity * CFps::Get()->DeltaTime());
		}
		Vector3 vec = m_vForce / (m_fMass == 0.0f ? 1.0f : m_fMass);
		m_vVel += vec * CFps::Get()->DeltaTime();
		pos += m_vVel * CFps::Get()->DeltaTime();
		// ��R
		m_fResistance = std::clamp(m_fResistance, 0.0f, 1.0f);
		m_vForce *= (1.0f - m_fResistance);

		// �ʒu�Œ�
		m_pFreezPos.Fix(pos);
	}

	// �p�x�Œ�
	m_pFreezRot.Fix(rot);

	GetOwner()->GetTransform()->SetPos(pos);
	GetOwner()->GetTransform()->SetRot(rot);

	// �������������ĂȂ���
	if (pos.x == oldPos.x && pos.y == oldPos.y && pos.z == oldPos.z)
	{
		m_bIsSleep = true;
	}
	else
	{
		m_bIsSleep = false;
	}

}

//==========================================================
// ���W�Œ�
// TODO: Init����Ăяo����Ȃ���΋@�\���Ȃ�
//==========================================================
void CRigidbody::SetFreezPos(bool x, bool y, bool z)
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();

	// �Œ�
	m_pFreezPos.Fixed(x, y, z, pos);
}

//==========================================================
// �p�x�Œ�
//==========================================================
void CRigidbody::SetFreezRot(bool x, bool y, bool z)
{
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	// �Œ�
	m_pFreezRot.Fixed(x, y, z, rot);
}


void CRigidbody::OnCollisionEnter(CGameObject* obj)
{
	//m_vForce = Vector3::zero();
	m_vVel = Vector3::zero();
}

void CRigidbody::OnCollisionStay(CGameObject* obj)
{
	//m_vForce = Vector3::zero();
	m_vVel = Vector3::zero();
}


#ifdef BUILD_MODE

void CRigidbody::ImGuiDebug()
{
	Debug::SetTextAndAligned(u8"Rigidbody �d��");
	ImGui::InputFloat("##Rigidbody �d��", &m_fGravity);

	Debug::SetTextAndAligned(u8"Rigidbody ��R");
	ImGui::InputFloat("##Rigidbody ��R", &m_fResistance);
	
	Debug::SetTextAndAligned(u8"Rigidbody ��");
	ImGui::InputFloat3("##Rigidbody ��", (float*)m_vForce);
	
	Debug::SetTextAndAligned(u8"Rigidbody ���x");
	ImGui::InputFloat3("##Rigidbody ���x", (float*)m_vVel);

	Debug::SetTextAndAligned(u8"Rigidbody �����x");
	ImGui::InputFloat3("##Rigidbody �����x", (float*)m_vAccel);

	/*Debug::SetTextAndAligned(u8"Rigidbody �ڕW���W");
	ImGui::InputFloat3("##Rigidbody �ڕW���W", (float*)m_vTargetPos);*/

	Debug::SetTextAndAligned(u8"Rigidbody Is Sleep");
	ImGui::Checkbox("##Rigidbody Is Sleep", &m_bIsSleep);

	Debug::SetTextAndAligned("Frieze Pos");
	ImGui::Checkbox(u8"x", &m_pFreezPos.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezPos.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezPos.bZ);
	m_pFreezPos.Fixed(GetOwner()->GetTransform()->GetPos());
	
	Debug::SetTextAndAligned("Frieze Rot");
	ImGui::Checkbox(u8"x", &m_pFreezRot.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezRot.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezRot.bZ);
	m_pFreezRot.Fixed(GetOwner()->GetTransform()->GetRot());
	
}

#endif // BUILD_MODE