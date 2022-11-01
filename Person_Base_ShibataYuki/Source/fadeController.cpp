//=========================================================
// [fadeController.cpp] 
// çÏê¨:2022/08/20
//---------------------------------------------------------
//=========================================================

#include <fadeController.h>

//
CFadeController::CFadeController(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_fFadeTime(60), m_eState(E_FadeState::NONE)
{

}
void CFadeController::Awake()
{
	/*if (CGameObject::FindGameObjectWithTag("Fade").lock())
		return;*/

	//CGameObject::DontDestroy(GetOwner(0));
	
	GetOwner()->SetName("FadeController");
	GetOwner()->GetTagPtr()->CreateTag("Fade");
	m_pPolygon = AddComponent<CPolygonRenderer>();
	m_pPolygon.lock()->SetColor(Color(0, 0, 0, 0));
	m_pPolygon.lock()->GetRectTransform()->SetSize(CScreen::GetWidth(), CScreen::GetHeight());
	GetOwner()->SetLayer(static_cast<int>(CLayer::E_Layer::FOG));
}
void CFadeController::Init()
{

}
void CFadeController::Update()
{
	switch (m_eState)
	{
	case CFadeController::E_FadeState::NONE:
		break;
	case CFadeController::E_FadeState::FADE_OUT_START:
		Call(m_eState);
		m_eState = E_FadeState::FADE_OUT;
		break;
	case CFadeController::E_FadeState::FADE_OUT:
	{
		auto color = m_pPolygon.lock()->GetColor();
		color.a += 1.0f / m_fFadeTime;				// ìßñæìx
		m_pPolygon.lock()->SetColor(color);
		if (color.a >= 1.0f)
		{
			m_eState = E_FadeState::NONE;
			Call(E_FadeState::FADE_OUT);
		}
		break;
	}
	case CFadeController::E_FadeState::FADE_IN_START:
		Call(m_eState);
		m_eState = E_FadeState::FADE_IN;
		break;
	case CFadeController::E_FadeState::FADE_IN:
	{
		auto color = m_pPolygon.lock()->GetColor();
		color.a -= 1.0f / m_fFadeTime*2;				// ìßñæìx
		m_pPolygon.lock()->SetColor(color);
		if (color.a <= 0.0f)
		{
			Call(m_eState);
			m_eState = E_FadeState::NONE;
		}
	}
	break;
	case CFadeController::E_FadeState::MAX:
		break;
	default:
		break;
	}
}
void CFadeController::StartFadeOut()
{
	if (m_eState == E_FadeState::FADE_OUT_START || m_eState == E_FadeState::FADE_OUT)
		return;
	m_pPolygon.lock()->SetColor({0,0,0,0});
	m_eState = E_FadeState::FADE_OUT_START;
}
void CFadeController::StartFadeIn()
{
	if (m_eState == E_FadeState::FADE_IN_START || m_eState == E_FadeState::FADE_IN)
		return;
	m_pPolygon.lock()->SetColor({0,0,0,1});
	m_eState = E_FadeState::FADE_IN_START;
}