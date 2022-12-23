//=========================================================
// �쐬:2022/05/24
// ����
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

//--- �萔��`
namespace 
{
	const float GRAVITY = 0.98f;		// �d�� ���W�n�ɂ���Č������Ⴄ
}

// �R���X�g���N�^
CRigidbody::CRigidbody()
	:m_bGravity(true), m_bIsSleep(false), m_fGravity(-GRAVITY), m_fResistance(1), m_fMass(1),
	m_vAccel(0, 0, 0), m_vTargetPos(1, 1, 1), m_vVel(0, 0, 0), m_vForce(0, 0, 0)
{

}
CRigidbody::CRigidbody(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bGravity(true), m_bIsSleep(false), m_fGravity(-GRAVITY), m_fResistance(1), m_fMass(1),
	m_vAccel(0,0,0),m_vTargetPos(0,0,0),m_vVel(0,0,0),m_vForce(0,0,0)
{

}
CRigidbody::~CRigidbody()
{

}
void CRigidbody::Update()
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();
	Vector3 oldPos = GetOwner()->GetTransform()->GetOldPos();
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	// �d�͂�^����
	if (m_bGravity)
	{
		//m_fResistance = std::clamp(m_fResistance, 0.f, 1.f);
		//m_vVel *= (1.0f - m_fResistance);
		//m_vVel.y += m_fGravity * CFps::Get()->DeltaTime();

		{
			m_vForce.y += m_fGravity * CFps::Get()->DeltaTime();
			Vector3 vec = m_vForce / m_fMass;
			m_vVel += vec * CFps::Get()->DeltaTime();
			pos += m_vVel * CFps::Get()->DeltaTime();

		}
	}

	// �ʒu�Œ�
	m_pFreezPos.Fix(pos);

	// �p�x�Œ�
	m_pFreezRot.Fix(rot);

	GetOwner()->GetTransform()->SetPos(pos);
	GetOwner()->GetTransform()->SetRot(rot);

	// �������������ĂȂ���
	if (pos.x == oldPos.x && pos.y == oldPos.y &&
		pos.z == oldPos.z)
	{
		m_bIsSleep = true;
	}
	else
	{
		m_bIsSleep = false;
	}

}
// TODO: Init����Ăяo����Ȃ���΋@�\���Ȃ�
void CRigidbody::SetFreezPos(bool x, bool y, bool z)
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();

	// �Œ�
	m_pFreezPos.Fixed(x, y, z, pos);
}
void CRigidbody::SetFreezRot(bool x, bool y, bool z)
{
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	// �Œ�
	m_pFreezRot.Fixed(x, y, z, rot);
}

#ifdef BUILD_MODE

void CRigidbody::ImGuiDebug()
{
	ImGui::InputFloat(u8"�d��", &m_fGravity);
	ImGui::InputFloat(u8"��R", &m_fResistance);
	ImGui::InputFloat3(u8"���x", (float*)m_vVel);
	ImGui::InputFloat3(u8"�����x", (float*)m_vAccel);
	ImGui::InputFloat3(u8"�ڕW���W", (float*)m_vTargetPos);
	ImGui::Checkbox(u8"��", &m_bIsSleep);

	ImGui::Text(u8"pos�Œ�");
	ImGui::SameLine();
	ImGui::Checkbox(u8"x", &m_pFreezPos.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezPos.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezPos.bZ);
	m_pFreezPos.Fix(GetOwner()->GetTransform()->GetPos());
	
	ImGui::Text(u8"rot�Œ�");
	ImGui::SameLine();
	ImGui::Checkbox(u8"x", &m_pFreezRot.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezRot.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezRot.bZ);
	m_pFreezRot.Fix(GetOwner()->GetTransform()->GetRot());
	
}
#endif // BUILD_MODE