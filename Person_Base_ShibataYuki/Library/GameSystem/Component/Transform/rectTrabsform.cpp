//=========================================================
// [rectTrabsform.h]
// �쐬:2022/06/28
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;

CRectTransform::CRectTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_vPos(0.0f, 0.0f), m_vSize(1, 1), m_fAngle(0.0f)
{

}
CRectTransform::~CRectTransform()
{

}
void CRectTransform::ImGuiDebug()
{
	// 3�������W
	//ImGui::InputFloat3(u8"�v���C���[���W", (float*)&this->GetPos());
	ImGui::DragFloat2(u8"���W", (float*)m_vPos);
	ImGui::DragFloat2(u8"�T�C�Y", (float*)m_vSize);
	ImGui::DragFloat(u8"�p�x", (float*)&m_fAngle);
	ImGui::SliderAngle(u8"�p�x", &m_fAngle);
}