//=========================================================
// [light.cpp]
// �쐬:2022/06/27
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Light/spotLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CSpotLight::CSpotLight()
{
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CSpotLight::CSpotLight(std::shared_ptr<CGameObject> owner)
	:CLight(owner), m_fRange(1)
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");

	m_vColor = Color(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f, 1.0f);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CSpotLight::~CSpotLight()
{
	
}

//==========================================================
// �ǂݍ��݌Ăяo��
//==========================================================
void CSpotLight::OnLoad()
{
	
}

//==========================================================
// �������Ăяo��
//==========================================================
void CSpotLight::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");
	
}


#ifdef BUILD_MODE

void CSpotLight::ImGuiDebug()
{
	CLight::ImGuiDebug();

	ImGui::Separator();

	Debug::SetTextAndAligned("Light Range");
	ImGui::DragFloat3("##Range", (float*)&m_fRange);
	
	Debug::SetTextAndAligned("Light Color");
	ImGui::DragFloat4("##Color", (float*)&m_vColor, 0.1f);
	
}

#endif // BUILD_MODE