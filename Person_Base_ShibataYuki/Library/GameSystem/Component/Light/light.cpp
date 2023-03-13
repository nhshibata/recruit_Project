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
	GetOwner()->SetObjTag("light");
	Set(this);
}

//==========================================================
// デストラクタ
//==========================================================
CLight::~CLight()
{
	if (m_pMainLight == this)
	{
		Set(nullptr);
	}

}

//==========================================================
// 読み込み呼び出し
//==========================================================
void CLight::OnLoad()
{
	if (!m_pMainLight)
	{
		m_pMainLight = this;
	}
	
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CLight::Awake()
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");
	Set(this);
	m_bEnable = true;
}

//==========================================================
// ﾒｲﾝﾗｲﾄの取得
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
// ﾒｲﾝﾗｲﾄ設定
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