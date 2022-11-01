//=========================================================
// [rectTrabsform.h]
// 作成:2022/06/28
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Transform/rectTransform.h>
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
	// 3次元座標
	//ImGui::InputFloat3(u8"プレイヤー座標", (float*)&this->GetPos());
	ImGui::SliderFloat2(u8"座標", (float*)m_vPos, -999, 999);
	ImGui::SliderFloat2(u8"サイズ", (float*)m_vSize, -999, 999);
	ImGui::SliderFloat(u8"角度", (float*)&m_fAngle, -999, 999);
	ImGui::SliderAngle(u8"角度", &m_fAngle);
}