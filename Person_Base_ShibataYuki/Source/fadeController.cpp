//=========================================================
// [fadeController.cpp] 
// �쐬:2022/08/20
//---------------------------------------------------------
//=========================================================

#include <fadeController.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Manager/sceneManager.h>

//
CFadeController::CFadeController(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_fFadeTime(60), m_eState(E_FadeState::NONE)
{
}

void CFadeController::Awake()
{
	m_SceneName.clear();

	GetOwner()->SetName("FadeController");
	GetOwner()->AddComponent<MySpace::Game::CPolygonRenderer>();
}

void CFadeController::Init()
{
	m_pPolygon = GetOwner()->GetComponent<MySpace::Game::CPolygonRenderer>();
	m_pPolygon.lock()->SetColor(Color(0, 0, 0, 0));
	m_pPolygon.lock()->GetRectTransform()->SetSize(CScreen::GetWidth(), CScreen::GetHeight());
	m_pPolygon.lock()->SetZ(static_cast<int>(CLayer::E_Layer::FOG));
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
		color.a += 1.0f / m_fFadeTime;				// �����x
		m_pPolygon.lock()->SetColor(color);
		if (color.a >= 1.0f)
		{
			Call(m_eState);
			m_eState = E_FadeState::NONE;
			
			//--- ����Scene�����w�肳��Ă����
			if (!m_SceneName.empty())
			{
				CSceneManager::Get().SceneTransition(m_SceneName);
				m_SceneName.clear();
				m_eState = E_FadeState::FADE_IN_START;
			}
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
		color.a -= 1.0f / m_fFadeTime*2;				// �����x
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

//========================================================
// Scene�ݒ�p
//========================================================
void CFadeController::SetScene(std::string sceneName)
{
	m_SceneName = sceneName;
	StartFadeOut();
}

//========================================================
// �t�F�[�h�A�E�g�J�n
//========================================================
void CFadeController::StartFadeOut()
{
	if (m_eState == E_FadeState::FADE_OUT_START || m_eState == E_FadeState::FADE_OUT)
		return;
	m_pPolygon.lock()->SetColor({ 0,0,0,0 });
	m_eState = E_FadeState::FADE_OUT_START;
}

//========================================================
// �t�F�[�h�C���J�n
//========================================================
void CFadeController::StartFadeIn()
{
	if (m_eState == E_FadeState::FADE_IN_START || m_eState == E_FadeState::FADE_IN)
		return;
	m_pPolygon.lock()->SetColor({ 0,0,0,1 });
	m_eState = E_FadeState::FADE_IN_START;
}