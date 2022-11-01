//=========================================================
// [light.cpp]
// 作成:2022/06/27
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Light/light.h>
#include <GameSystem/GameObject/gameObject.h>
#include <ImGui/imgui.h>
#include <GameSystem/Manager/sceneManager.h>

using namespace MySpace::Game;

//CLight g_Light;
CLight* CLight::m_pLight = nullptr;

CLight::CLight()
	:m_bEnable(true)
{
}
CLight::CLight(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{
	//m_pLight = this;
	//StartUp();
}
CLight::~CLight()
{

}
void CLight::Init()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->GetTagPtr()->SetTag("light");

	m_bEnable = true;
}
void CLight::Update()
{
}
CLight* CLight::Get()
{
	using namespace::MySpace::SceneManager;
	if (!m_pLight)
	{
		if (auto obj = CGameObject::FindGameObjectWithTag("light"); obj.lock())
		{
			m_pLight = obj.lock()->GetComponent<CLight>().lock().get();
		}
	}
	return m_pLight;
}
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