//=========================================================
// [light.cpp]
// �쐬:2022/06/27
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Light/light.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;


//==========================================================
// �R���X�g���N�^
//==========================================================
CLight::CLight()
	:m_bEnable(true)
{
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CLight::CLight(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bEnable(true)
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetTag("light");
	Set(this);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLight::~CLight()
{
	Set(nullptr);
}

//==========================================================
// �������Ăяo��
//==========================================================
void CLight::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetTag("light");
	Set(this);
	m_bEnable = true;
}

//==========================================================
// Ҳ�ײĂ̎擾
//==========================================================
CLight* CLight::GetMain()
{
	using namespace::MySpace::SceneManager;
	if (!m_pLight)
	{
		if (auto obj = CGameObject::FindGameObjectWithTag("light"); obj.lock())
		{
			if (m_pLight = obj.lock()->GetComponent<CLight>().lock().get(); !m_pLight)
				m_pLight = obj.lock()->GetComponent<CDirectionalLight>().lock().get();
		}
	}
	return m_pLight;
}

//==========================================================
// Ҳ�ײĐݒ�
//==========================================================
void CLight::Set(CLight* pLight)
{
	//m_pLight = (pLight) ? pLight : &g_Light;
	m_pLight = (pLight);
}


#ifdef BUILD_MODE

void CLight::ImGuiDebug()
{
	ImGui::Text(u8"*Light");
	ImGui::CheckboxFlags(u8"enable", (unsigned int*)&m_bEnable, 1);
}

#endif // BUILD_MODE