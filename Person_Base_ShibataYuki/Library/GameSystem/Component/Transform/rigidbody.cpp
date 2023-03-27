//=========================================================
// [rigidbody.cpp]
//---------------------------------------------------------
// 作成:2022/05/24
// 更新:2023/01/31 異常な更新が起きていたため、調査
//				FixedUpdateの呼び出し条件の修正ミス
//---------------------------------------------------------
// 物理
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//--- 定数定義
namespace 
{
	const float GRAVITY = -9.8f;		// 重力 座標系によって向きが違う
}

//==========================================================
// コンストラクタ
//==========================================================
CRigidbody::CRigidbody()
{
}

//==========================================================
// コンストラクタ
//==========================================================
CRigidbody::CRigidbody(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
	, m_bGravity(true), m_bIsSleep(false), m_fGravity(GRAVITY), m_fResistance(1.0f), m_fMass(1.0f),
	m_vAccel(0, 0, 0), m_vTargetPos(1, 1, 1), m_vVel(0, 0, 0), m_vForce(0, 0, 0)
{

}

//==========================================================
// デストラクタ
//==========================================================
CRigidbody::~CRigidbody()
{

}

//==========================================================
// 更新
//==========================================================
void CRigidbody::FixedUpdate()
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();
	Vector3 oldPos = GetOwner()->GetTransform()->GetOldPos();
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	{
		// 重力を与える
		if (m_bGravity)
		{
			m_vForce.y += float(m_fGravity * CFps::Get()->DeltaTime());
		}
		Vector3 vec = m_vForce / (m_fMass == 0.0f ? 1.0f : m_fMass);
		m_vVel += vec * CFps::Get()->DeltaTime();
		pos += m_vVel * CFps::Get()->DeltaTime();
		// 抵抗
		m_fResistance = std::clamp(m_fResistance, 0.0f, 1.0f);
		m_vForce *= (1.0f - m_fResistance);

		// 位置固定
		m_pFreezPos.Fix(pos);
	}

	// 角度固定
	m_pFreezRot.Fix(rot);

	GetOwner()->GetTransform()->SetPos(pos);
	GetOwner()->GetTransform()->SetRot(rot);

	// 動いたか動いてないか
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
// 座標固定
// TODO: Initから呼び出されなければ機能しない
//==========================================================
void CRigidbody::SetFreezPos(bool x, bool y, bool z)
{
	Vector3 pos = GetOwner()->GetTransform()->GetPos();

	// 固定
	m_pFreezPos.Fixed(x, y, z, pos);
}

//==========================================================
// 角度固定
//==========================================================
void CRigidbody::SetFreezRot(bool x, bool y, bool z)
{
	Vector3 rot = GetOwner()->GetTransform()->GetRot();

	// 固定
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
	Debug::SetTextAndAligned(u8"Rigidbody 重力");
	ImGui::InputFloat("##Rigidbody 重力", &m_fGravity);

	Debug::SetTextAndAligned(u8"Rigidbody 抵抗");
	ImGui::InputFloat("##Rigidbody 抵抗", &m_fResistance);
	
	Debug::SetTextAndAligned(u8"Rigidbody 力");
	ImGui::InputFloat3("##Rigidbody 力", (float*)m_vForce);
	
	Debug::SetTextAndAligned(u8"Rigidbody 速度");
	ImGui::InputFloat3("##Rigidbody 速度", (float*)m_vVel);

	Debug::SetTextAndAligned(u8"Rigidbody 加速度");
	ImGui::InputFloat3("##Rigidbody 加速度", (float*)m_vAccel);

	/*Debug::SetTextAndAligned(u8"Rigidbody 目標座標");
	ImGui::InputFloat3("##Rigidbody 目標座標", (float*)m_vTargetPos);*/

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