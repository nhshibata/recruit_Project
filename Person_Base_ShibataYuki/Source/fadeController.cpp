//=========================================================
// [fadeController.cpp] 
// 作成:2022/08/20
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <fadeController.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

#include <Application/Application.h>
#include <tchar.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CFadeController::CFadeController()
	:m_fFadeTime(60), m_eState(E_FadeState::NONE)
{
	
}


//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CFadeController::CFadeController(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
	,m_fFadeTime(60), m_eState(E_FadeState::NONE)
{
	m_SceneName.clear();

}

//==========================================================
// デスクトラクタ
//==========================================================
CFadeController::~CFadeController()
{
	
}

void CFadeController::Awake()
{
	//m_SceneName.clear();
	GetOwner()->SetName("FadeController");
}

void CFadeController::Init()
{
	m_pPolygon = GetOwner()->GetComponent<CPolygonRenderer>();

	// NOTE:ここで呼び出すとバグ発生
	//if (!m_pPolygon.lock())
	//	AddComponent<CPolygonRenderer>();
	
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
		color.a += 1.0f / m_fFadeTime;				// 透明度
		m_pPolygon.lock()->SetColor(color);
		if (color.a >= 1.0f)
		{
			Call(m_eState);
			m_eState = E_FadeState::NONE;
			
			//--- 次のScene名が指定されていれば
			if (!m_SceneName.empty())
			{
				CSceneManager::Get()->SceneTransition(m_SceneName);
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
		color.a -= 1.0f / m_fFadeTime*2;				// 透明度
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
// Scene設定用
//========================================================
void CFadeController::SetScene(std::string sceneName)
{
	m_SceneName = sceneName;
	StartFadeOut();
}

//========================================================
// フェードアウト開始
//========================================================
void CFadeController::StartFadeOut()
{
	if (m_eState == E_FadeState::FADE_OUT_START || m_eState == E_FadeState::FADE_OUT)
		return;
	m_pPolygon.lock()->SetColor({ 0,0,0,0 });
	m_eState = E_FadeState::FADE_OUT_START;
}

//========================================================
// フェードイン開始
//========================================================
void CFadeController::StartFadeIn()
{
	if (m_eState == E_FadeState::FADE_IN_START || m_eState == E_FadeState::FADE_IN)
		return;
	m_pPolygon.lock()->SetColor({ 0,0,0,1 });
	m_eState = E_FadeState::FADE_IN_START;
}

//========================================================
// 状態による関数ポインタ呼び出し
//========================================================
void CFadeController::Call(E_FadeState state)
{
	if (m_aLoopFuncMap[state].size() != 0)
	{// 繰り返し
		for (auto & func : m_aLoopFuncMap[state])
		{
			func();
		}
	}

	// 一回
	if (m_aFuncMap.size() == 0)
		return;

	for (auto & func : m_aFuncMap[state])
	{
		func();
	}
	m_aFuncMap[state].clear();
}