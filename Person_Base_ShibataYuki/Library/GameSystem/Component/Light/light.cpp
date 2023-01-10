//=========================================================
// [light.cpp]
// 作成:2022/06/27
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Light/light.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;


//==========================================================
// コンストラクタ
//==========================================================
CLight::CLight()
	:m_bEnable(true)
{
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CLight::CLight(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bEnable(true)
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetTag("light");
	Set(this);
}

//==========================================================
// デストラクタ
//==========================================================
CLight::~CLight()
{
	Set(nullptr);
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CLight::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetTag("light");
	Set(this);
	m_bEnable = true;
}

//==========================================================
// ﾒｲﾝﾗｲﾄの取得
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
// ﾒｲﾝﾗｲﾄ設定
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