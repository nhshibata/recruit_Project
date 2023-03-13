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
		Set(nullptr);
	}

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
	//m_pLight = (pLight) ? pLight : &g_Light;
	m_pMainLight = (pLight);
}


#ifdef BUILD_MODE

void CLight::ImGuiDebug()
{
	ImGui::Text("Light");
	ImGui::CheckboxFlags(u8"enable", (unsigned int*)&m_bEnable, 1);
}

#endif // BUILD_MODE