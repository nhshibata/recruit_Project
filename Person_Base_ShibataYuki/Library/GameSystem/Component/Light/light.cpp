//=========================================================
// [light.cpp]
// �쐬:2022/06/27
// 
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Component/Light/light.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/lightManager.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLight::CLight()
	:m_nSystemIdx(-1)
{
	// �V�X�e���ւ̓o�^
	// ���[�h�p
	if(m_nSystemIdx == -1)
		m_nSystemIdx = CLightManager::Get()->RegistToSystem(BaseToDerived<CLight>());
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CLight::CLight(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bEnable(true), m_nSystemIdx(-1)
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");
	Set(this);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLight::~CLight()
{
	if (m_pMainLight == this)
	{
		m_pMainLight = nullptr;
	}

	if(m_nSystemIdx != -1)
		CLightManager::Get()->ExecutSystem(m_nSystemIdx);
}

//==========================================================
// �ǂݍ��݌Ăяo��
//==========================================================
void CLight::OnLoad()
{
	if (!m_pMainLight)
	{
		m_pMainLight = this;
	}
}

//==========================================================
// �������Ăяo��
//==========================================================
void CLight::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");
	Set(this);
	m_bEnable = true;

	if (m_nSystemIdx == -1)
		m_nSystemIdx = CLightManager::Get()->RegistToSystem(BaseToDerived<CLight>());

}

//==========================================================
// Ҳ�ײĂ̎擾
//==========================================================
CLight* CLight::GetMain()
{
	using namespace::MySpace::SceneManager;
	if (!m_pMainLight)
	{
		if (auto obj = CGameObject::FindGameObjectWithTag("light"); obj.lock())
		{
			if (m_pMainLight = obj.lock()->GetComponent<CLight>().lock().get(); !m_pMainLight)
				m_pMainLight = obj.lock()->GetComponent<CDirectionalLight>().lock().get();
		}
	}
	return m_pMainLight;
}

//==========================================================
// Ҳ�ײĐݒ�
//==========================================================
void CLight::Set(CLight* pLight)
{	
	if (m_pMainLight)
		return;

	m_pMainLight = pLight;
}


#ifdef BUILD_MODE

void CLight::ImGuiDebug()
{
	Debug::SetTextAndAligned("Main Light");
	if (ImGui::Button("ON"))
	{
		m_pMainLight = this;
	}

	Debug::SetTextAndAligned("Light Enable");
	ImGui::CheckboxFlags("##Light Enable", (unsigned int*)&m_bEnable, 1);
}

#endif // BUILD_MODE