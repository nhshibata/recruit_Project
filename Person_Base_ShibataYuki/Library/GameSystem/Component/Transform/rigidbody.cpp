//=========================================================
// 作成:2022/05/24
// 物理
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

//--- 定数定義
namespace 
{
	const float GRAVITY = 0.98f;		// 重力 座標系によって向きが違う
}

// コンストラクタ
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

	// 重力を与える
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

	// 位置固定
	m_pFreezPos.Fix(pos);

	// 角度固定
	m_pFreezRot.Fix(rot);

	GetOwner()->GetTransform()->SetPos(pos);
	GetOwner()->GetTransform()->SetRot(rot);

	// 動いたか動いてないか
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
// TODO: Initから呼び出されなければ機能しない
void CRigidbody::SetFreezPos(bool x, bool y, bool z)
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();

	// 固定
	m_pFreezPos.Fixed(x, y, z, pos);
}
void CRigidbody::SetFreezRot(bool x, bool y, bool z)
{
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	// 固定
	m_pFreezRot.Fixed(x, y, z, rot);
}

#ifdef BUILD_MODE

void CRigidbody::ImGuiDebug()
{
	ImGui::InputFloat(u8"重力", &m_fGravity);
	ImGui::InputFloat(u8"抵抗", &m_fResistance);
	ImGui::InputFloat3(u8"速度", (float*)m_vVel);
	ImGui::InputFloat3(u8"加速度", (float*)m_vAccel);
	ImGui::InputFloat3(u8"目標座標", (float*)m_vTargetPos);
	ImGui::Checkbox(u8"動", &m_bIsSleep);

	ImGui::Text(u8"pos固定");
	ImGui::SameLine();
	ImGui::Checkbox(u8"x", &m_pFreezPos.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezPos.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezPos.bZ);
	m_pFreezPos.Fix(GetOwner()->GetTransform()->GetPos());
	
	ImGui::Text(u8"rot固定");
	ImGui::SameLine();
	ImGui::Checkbox(u8"x", &m_pFreezRot.bX);
	ImGui::SameLine();
	ImGui::Checkbox(u8"y", &m_pFreezRot.bY);
	ImGui::SameLine();
	ImGui::Checkbox(u8"z", &m_pFreezRot.bZ);
	m_pFreezRot.Fix(GetOwner()->GetTransform()->GetRot());
	
}
#endif // BUILD_MODE