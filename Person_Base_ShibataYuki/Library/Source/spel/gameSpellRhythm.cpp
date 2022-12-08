//=========================================================
// [gameSpellRhythm.cpp] 
// 作成:2022/09/20
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <gameSpellRhythm.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <CoreSystem/Input/Keyboard.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>

// ゲーム
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <gameCamera.h>

// UI
#include <fadeController.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;
using namespace Spell;

CGameSpellRhythm::CGameSpellRhythm(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_bPause(false)
{
}
void CGameSpellRhythm::Awake() 
{
	//--- 実質ここから始まる
	GetOwner()->SetName("GameManager");
	GetOwner()->GetTagPtr()->CreateTag("GameManager");

	CGameObject::DontDestroy(GetOwner(0));

	CSceneManager::Get().GetActiveScene()->SetSceneName("Title");
	auto obj = CGameObject::CreateObject().lock();
	m_pFade = obj->AddComponent<CFadeController>();
	
	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CTitleSceneManager>();
	
	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CDirectionalLight>();
	CGameObject::DontDestroy(obj);

	// ゲームシーン用ｶﾒﾗ
	obj = CGameObject::CreateObject().lock();
	m_pGameCamera = obj->AddComponent<Spell::CGameCamera>();
	//CGameObject::DontDestroy(obj);
	
	m_pFade.lock()->SetTypeFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_OUT_START, &CGameSpellRhythm::FadeFuncPtr, this);
	/*m_pFade.lock()->SetFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_OUT_START, FadeFuncPtr, this, true);
	m_pFade.lock()->SetFunction(CFadeController::E_FadeState::FADE_IN_START, FadeFuncPtr, true);*/
}
void CGameSpellRhythm::Init()  
{
	// 関数のセット
	CSceneManager::Get().SceneChanged<CGameSpellRhythm>(&CGameSpellRhythm::CreateManager, this);
	CSound::Play(eBGM::BGM_TITLE);
}
void CGameSpellRhythm::Update()
{
	
	
	//if (MySpace::Input::Keyboad::IsTrigger(VK_K))
	//{
	//	m_bPause ^= true;
	//	Pause();
	//}
}
void CGameSpellRhythm::FixedUpdate()
{
	// ｶﾒﾗ切替
	// debugCameraの方で切替を実装するべきか?
	auto name = CCamera::GetMain()->GetName();
	if (MySpace::Input::Keyboad::IsTrigger(VK_END) && m_pDebugCamera.lock())
	{
		CCamera::GetMain()->SetMain(m_pDebugCamera);
		m_pDebugCamera.lock()->SetActive(true);
		
		if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
		{
			com->SetActive(true);
		}
		if (m_pGameCamera.lock())
		{
			m_pGameCamera.lock()->SetActive(false);
			if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}
	}
	if (MySpace::Input::Keyboad::IsTrigger(VK_HOME) && m_pGameCamera.lock())
	{
		CCamera::GetMain()->SetMain(m_pGameCamera);
		m_pGameCamera.lock()->SetActive(true);
		if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pDebugCamera.lock())
		{
			m_pDebugCamera.lock()->SetActive(false);
			if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}
	}
}
void CGameSpellRhythm::Pause()
{
	auto state = CGameObject::E_ObjectState::ACTIVE;

	if (m_bPause)
		state = CGameObject::E_ObjectState::WAIT;

	ObjStateSwitch(state, CSceneManager::Get().GetActiveScene());
}
void CGameSpellRhythm::FadeFuncPtr()
{
	CGameObject::E_ObjectState state = CGameObject::E_ObjectState::WAIT;
	// fade in/out 開始時には停止状態にする
	if (m_pFade.lock()->GetState() == CFadeController::E_FadeState::FADE_IN)
		state = CGameObject::E_ObjectState::ACTIVE;

	ObjStateSwitch(state, MySpace::SceneManager::CSceneManager::Get().GetActiveScene());
}
// 停止/行動状態に変更
void CGameSpellRhythm::ObjStateSwitch(CGameObject::E_ObjectState state, CScene* scene)
{
	auto list = scene->GetObjManager()->GetList();
	for (auto & pObj : list)
	{
		// 自身は除外
		if (pObj == GetOwner(0).lock() || pObj == m_pFade.lock()->GetOwner(0).lock())
		{
			continue;
		}
		if (state == CGameObject::E_ObjectState::ACTIVE)
		{
			if (pObj->GetState() == CGameObject::E_ObjectState::WAIT)
				pObj->SetState(state);
		}
		if (state == CGameObject::E_ObjectState::WAIT)
		{
			if (pObj->GetState() == CGameObject::E_ObjectState::ACTIVE)
				pObj->SetState(state);
		}
	}
}
// シーン遷移呼び出し
void* CGameSpellRhythm::CreateManager(CScene* prev, CScene* next)
{
	auto obj = CGameObject::CreateObject().lock();
	m_pFade = obj->AddComponent<CFadeController>();

	if (!m_pGameCamera.lock())
	{
		obj = CGameObject::CreateObject().lock();
		m_pGameCamera = obj->AddComponent<Spell::CGameCamera>();
	}

	for (int cnt = 0; cnt < eBGM::MAX_BGM; ++cnt)
	{
		CSound::Stop(static_cast<eBGM>(cnt));
	}

	// 管理オブジェクトを生成
	// ゲーム
	if (next->GetSceneName() == "Game")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CGameSceneManager>();

		// デバッグｶﾒﾗ
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();

		// メインｶﾒﾗ設定
		CCamera::GetMain()->SetMain(m_pGameCamera);
		m_pGameCamera.lock()->SetActive(true);
		if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pDebugCamera.lock())
		{
			m_pDebugCamera.lock()->SetActive(false);
			if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}

		CSound::Play(eBGM::BGM_GAME);
	}
	
	if (next->GetSceneName() == CTutrialSceneManager::SCENE_NAME)
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CTutrialSceneManager>();

		// デバッグｶﾒﾗ
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();

		// メインｶﾒﾗ設定
		CCamera::GetMain()->SetMain(m_pGameCamera);
		m_pGameCamera.lock()->SetActive(true);
		if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pDebugCamera.lock())
		{
			m_pDebugCamera.lock()->SetActive(false);
			if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}

		CSound::Play(eBGM::BGM_TITLE);
	}
	// リザルト⇒タイトル
	if (next->GetSceneName() == "Title")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CTitleSceneManager>();
		CSound::Play(eBGM::BGM_TITLE);
	}
	// ゲーム⇒リザルト
	if (next->GetSceneName() == "Result")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CResultSceneManager>();
		if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(false);
		CSound::Play(eBGM::BGM_RESULT);
	}

	// 次のシーンのobjを停止状態に
	ObjStateSwitch(CGameObject::E_ObjectState::WAIT, next);
	// フェードイン終了後
	m_pFade.lock()->SetTypeFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_IN, &CGameSpellRhythm::FadeFuncPtr, this);
	m_pFade.lock()->StartFadeIn();

	// 再帰
	CSceneManager::Get().SceneChanged<CGameSpellRhythm>(&CGameSpellRhythm::CreateManager, this);
	return nullptr;
}
//=====================================================
// 派生ｸﾗｽで呼び出す必要あり
void CSceneManagerComponent::Init()
{
	if (auto obj = CGameObject::FindGameObjectWithTag("Fade").lock(); obj)
	{
		m_pFade = obj->GetComponent<CFadeController>();
	}

	// シーン遷移セット
	// fadeout終了時呼び出し
	if(m_pFade.lock())
		m_pFade.lock()->SetTypeFunction<CSceneManagerComponent>(CFadeController::E_FadeState::FADE_OUT, &CSceneManagerComponent::FuncPtr, this);
};
void CSceneManagerComponent::FuncPtr()
{	// シーン遷移設定
	SetScene(m_NextSceneName);
}
void CSceneManagerComponent::SetScene(std::string next)
{
	CSceneManager::Get().SceneTransition(next);
}
//=====================================================
void CTitleSceneManager::Awake()
{
	GetOwner()->SetName("TitleManager");
	auto obj = CGameObject::CreateObject().lock();
	//obj->AddComponent<CTitleUI>();

	//obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CTextRenderer>();

	obj = CGameObject::CreateObject().lock();
	auto com = obj->AddComponent<CTextRenderer>();
	com->SetTextWString(L"チュートリアル[J]Tutorial");
	com->SetColor({ 0,0,0,1 });
	com->GetRectTransform()->SetPos(210, -310);
}
void CTitleSceneManager::Init()
{
	CSceneManagerComponent::Init();
	auto obj = CGameObject::FindGameObjectWithTag("GameManager");
	m_pGameManager = obj.lock()->GetComponent<CGameSpellRhythm>();

}
void CTitleSceneManager::Update()
{
	if (MySpace::Input::Keyboad::IsTrigger(VK_W) || MySpace::Input::Keyboad::IsPress(VK_W))
	{

	}
	if (MySpace::Input::Keyboad::IsTrigger(VK_S) || MySpace::Input::Keyboad::IsPress(VK_S))
	{

	}
	if (MySpace::Input::Keyboad::IsTrigger(VK_SPACE))
	{
		CSound::Play(eSE::SE_ENTER);
		m_pFade.lock()->StartFadeOut();
	}
	
	if (MySpace::Input::Keyboad::IsTrigger(VK_J))
	{
		CSound::Play(eSE::SE_ENTER);
		m_NextSceneName = CTutrialSceneManager::SCENE_NAME;
		m_pFade.lock()->StartFadeOut();
	}
}
//=========================================================
void CGameSceneManager::Awake()
{
	GetOwner()->SetName("GameSceneManager");

	CSceneManagerComponent::Awake();

	auto pObj = CGameObject::FindGameObjectWithTag("GameManager");
	auto game = pObj.lock()->GetComponent<CGameSpellRhythm>().lock();

}
void CGameSceneManager::Init()
{
	// ポインタの取得
	CSceneManagerComponent::Init();
}
void CGameSceneManager::Update()
{
	// 終了準備
	if (m_bEnd)
	{

	}
}
//=======================================================
void CResultSceneManager::Awake()
{
	GetOwner()->SetName("ResultSceneManager");
	auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
	auto mgr = mgrO.lock()->GetComponent<CGameSpellRhythm>();
		

}
void CResultSceneManager::Init()
{
	CSceneManagerComponent::Init();
}
void CResultSceneManager::Update()
{
	if (MySpace::Input::Keyboad::IsTrigger(VK_SPACE))
	{
		m_pFade.lock()->StartFadeOut();
		CSound::Play(eSE::SE_ENTER);
	}
}

//=========================================================
void CTutrialSceneManager::Awake()
{
	GetOwner()->SetName("GameSceneManager");

	CSceneManagerComponent::Awake();
	
	Vector2 pos = {-CScreen::GetWidth()/2.5f, 200};
	LPCWSTR Text[5] = {
		L"[A]rotLeft",
		L"[D]rotRight",
		L"[W]speedUp",
		L"[S]speedDown",
		L"[Space]End",
	};
	for (int cnt = 0; cnt < 5; ++cnt)
	{
		auto obj = CGameObject::CreateObject().lock();
		auto text = obj->AddComponent<CTextRenderer>();
		text->SetTextWString(Text[cnt]);
		text->SetColor({ 0, 0, 0, 1 });
		text->GetRectTransform()->SetPos(pos);
		pos.y -= 100;
	}

}
void CTutrialSceneManager::Init()
{
	// ポインタの取得
	CSceneManagerComponent::Init();
}
void CTutrialSceneManager::Update()
{
	// ゲーム終了条件などを確認

	if (MySpace::Input::Keyboad::IsTrigger(VK_SPACE))
	{
		m_pFade.lock()->StartFadeOut();
		CSound::Play(eSE::SE_ENTER);
	}

	
	// 終了準備
	if (m_bEnd)
	{
		auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
		auto mgr = mgrO.lock()->GetComponent<CGameSpellRhythm>();
	}
}
