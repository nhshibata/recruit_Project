//=========================================================
// [rectTrabsform.h]
// �쐬:2022/06/28
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CRectTransform::CRectTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_vPos(0.0f, 0.0f), m_vSize(1, 1), m_fAngle(0.0f)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CRectTransform::~CRectTransform()
{

}


#ifdef BUILD_MODE

void CRectTransform::ImGuiDebug()
{
	// 3�������W
	
	Debug::SetTextAndAligned(u8"rect pos");
	ImGui::DragFloat2("##rect pos", (float*)m_vPos);
	
	Debug::SetTextAndAligned("rect size");
	ImGui::DragFloat2("##rect size", (float*)m_vSize);
	
	Debug::SetTextAndAligned("rect angle");
	ImGui::DragFloat("##rect angle", (float*)&m_fAngle);
	
	Debug::SetTextAndAligned("rect amgle");
	ImGui::SliderAngle("##rect angle", &m_fAngle);

}

#endif // BUILD_MODE