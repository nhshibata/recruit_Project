//=========================================================
// [gameScene.cpp] 
// �쐬:2022/08/18
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <gameScene.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <CoreSystem/Input/Keyboard.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>

// �Q�[��
#include <centipede.h>
#include <itemBase.h>
#include <ground.h>
#include <enemyBase.h>
#include <bulletBase.h>
#include <myCamera.h>
#include <timer.h>
#include <GameSystem/Component/Collision/boxCollision.h>

// UI
#include <titleUI.h>
#include <gameUI.h>
#include <resultUI.h>

using namespace MySpace::SceneManager;

CGameCentipedeMarch::CGameCentipedeMarch(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_bPause(false)
{
}
void CGameCentipedeMarch::Awake() 
{
	//--- ������������n�܂�

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

	// �Q�[���V�[���p���
	obj = CGameObject::CreateObject().lock();
	m_pMyCamera = obj->AddComponent<CMyCamera>();
	CGameObject::DontDestroy(obj);
	
	m_pFade.lock()->SetTypeFunction<CGameCentipedeMarch>(CFadeController::E_FadeState::FADE_OUT_START, &CGameCentipedeMarch::FadeFuncPtr, this);
	/*m_pFade.lock()->SetFunction<CGameManager>(CFadeController::E_FadeState::FADE_OUT_START, FadeFuncPtr, this, true);
	m_pFade.lock()->SetFunction(CFadeController::E_FadeState::FADE_IN_START, FadeFuncPtr, true);*/
}
void CGameCentipedeMarch::Init()  
{
	// �֐��̃Z�b�g
	CSceneManager::Get().SceneChanged<CGameCentipedeMarch>(&CGameCentipedeMarch::CreateManager, this);
	CSound::Play(eBGM::BGM_TITLE);
}
void CGameCentipedeMarch::Update()
{
	
	
	//if (MySpace::Input::Keyboad::IsTrigger(VK_K))
	//{
	//	m_bPause ^= true;
	//	Pause();
	//}
}
void CGameCentipedeMarch::FixedUpdate()
{
	// ��אؑ�
	// debugCamera�̕��Őؑւ���������ׂ���?
	auto name = CCamera::GetMain()->GetName();
	if (MySpace::Input::Keyboad::IsTrigger(VK_END) && m_pDebugCamera.lock())
	{
		CCamera::GetMain()->Set(m_pDebugCamera);
		m_pDebugCamera.lock()->SetActive(true);
		if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pMyCamera.lock())
		{
			m_pMyCamera.lock()->SetActive(false);
			if (auto com = m_pMyCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}
	}
	if (MySpace::Input::Keyboad::IsTrigger(VK_HOME) && m_pMyCamera.lock())
	{
		CCamera::GetMain()->Set(m_pMyCamera);
		m_pMyCamera.lock()->SetActive(true);
		if (auto com = m_pMyCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pDebugCamera.lock())
		{
			m_pDebugCamera.lock()->SetActive(false);
			if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}
	}
}
void CGameCentipedeMarch::Pause()
{
	auto state = CGameObject::E_ObjectState::ACTIVE;

	if (m_bPause)
		state = CGameObject::E_ObjectState::WAIT;

	ObjStateSwitch(state, CSceneManager::Get().GetActiveScene());
}
void CGameCentipedeMarch::FadeFuncPtr()
{
	CGameObject::E_ObjectState state = CGameObject::E_ObjectState::WAIT;
	// fade in/out �J�n���ɂ͒�~��Ԃɂ���
	if (m_pFade.lock()->GetState() == CFadeController::E_FadeState::FADE_IN)
		state = CGameObject::E_ObjectState::ACTIVE;

	ObjStateSwitch(state, MySpace::SceneManager::CSceneManager::Get().GetActiveScene());
}
// ��~/�s����ԂɕύX
void CGameCentipedeMarch::ObjStateSwitch(CGameObject::E_ObjectState state, CScene* scene)
{
	auto list = scene->GetObjManager()->GetList();
	for (auto & pObj : list)
	{
		// ���g�͏��O
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
void* CGameCentipedeMarch::CreateManager(CScene* prev, CScene* next)
{
	auto obj = CGameObject::CreateObject().lock();
	m_pFade = obj->AddComponent<CFadeController>();

	if (!m_pMyCamera.lock())
	{
		obj = CGameObject::CreateObject().lock();
		m_pMyCamera = obj->AddComponent<CMyCamera>();
	}

	for (int cnt = 0; cnt < eBGM::MAX_BGM; ++cnt)
	{
		CSound::Stop(static_cast<eBGM>(cnt));
	}

	// �Ǘ��I�u�W�F�N�g�𐶐�
	// �Q�[��
	//if (prev->GetSceneName() == "Title" && next->GetSceneName() == "Game")
	if (next->GetSceneName() == "Game")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CGameSceneManager>();

		// �f�o�b�O���
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();
		m_pFade.lock()->SetTypeFunction<CMyCamera>(CFadeController::E_FadeState::FADE_IN, &CMyCamera::FindPlayer, m_pMyCamera.lock().get());

		// ���C����אݒ�
		CCamera::GetMain()->Set(m_pMyCamera);
		m_pMyCamera.lock()->SetActive(true);
		if (auto com = m_pMyCamera.lock()->GetComponent<CModelRenderer>(); com)
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

		// �f�o�b�O���
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();
		m_pFade.lock()->SetTypeFunction<CMyCamera>(CFadeController::E_FadeState::FADE_IN, &CMyCamera::FindPlayer, m_pMyCamera.lock().get());

		// ���C����אݒ�
		CCamera::GetMain()->Set(m_pMyCamera);
		m_pMyCamera.lock()->SetActive(true);
		if (auto com = m_pMyCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(true);

		if (m_pDebugCamera.lock())
		{
			m_pDebugCamera.lock()->SetActive(false);
			if (auto com = m_pDebugCamera.lock()->GetComponent<CModelRenderer>(); com)
				com->SetActive(false);
		}

		CSound::Play(eBGM::BGM_TITLE);
	}
	// ���U���g�˃^�C�g��
	if (next->GetSceneName() == "Title")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CTitleSceneManager>();
		CSound::Play(eBGM::BGM_TITLE);
	}
	// �Q�[���˃��U���g
	if (next->GetSceneName() == "Result")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CResultSceneManager>();
		if (auto com = m_pMyCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(false);
		CSound::Play(eBGM::BGM_RESULT);
	}

	// ���̃V�[����obj���~��Ԃ�
	ObjStateSwitch(CGameObject::E_ObjectState::WAIT, next);
	// �t�F�[�h�C���I����
	m_pFade.lock()->SetTypeFunction<CGameCentipedeMarch>(CFadeController::E_FadeState::FADE_IN, &CGameCentipedeMarch::FadeFuncPtr, this);
	//m_pFade.lock()->SetTypeFunction<CFadeController>(CFadeController::E_FadeState::FADE_OUT, &CFadeController::StartFadeIn, m_pFade.lock().get());
	m_pFade.lock()->StartFadeIn();

	// �ċA
	//CSceneManager::Get().SceneChanged(std::bind(&CGameManager::CreateManager, this, std::placeholders::_1, std::placeholders::_2));
	CSceneManager::Get().SceneChanged<CGameCentipedeMarch>(&CGameCentipedeMarch::CreateManager, this);
	return nullptr;
}
//=====================================================
// �h���׽�ŌĂяo���K�v����
void CSceneManagerComponent::Init()
{
	if (auto obj = CGameObject::FindGameObjectWithTag("Fade").lock(); obj)
	{
		m_pFade = obj->GetComponent<CFadeController>();
	}

	// �V�[���J�ڃZ�b�g
	// fadeout�I�����Ăяo��
	if(m_pFade.lock())
		m_pFade.lock()->SetTypeFunction<CSceneManagerComponent>(CFadeController::E_FadeState::FADE_OUT, &CSceneManagerComponent::FuncPtr, this);
};
void CSceneManagerComponent::FuncPtr()
{	// �V�[���J�ڐݒ�
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
	obj->AddComponent<CTitleUI>();

	obj = CGameObject::CreateObject().lock();
	m_pNum = obj->AddComponent<CTextRenderer>();

	obj = CGameObject::CreateObject().lock();
	auto com = obj->AddComponent<CTextRenderer>();
	com->SetTextWString(L"�`���[�g���A��[J]Tutorial");
	com->SetColor({ 0,0,0,1 });
	com->GetRectTransform()->SetPos(210, -310);
}
void CTitleSceneManager::Init()
{
	CSceneManagerComponent::Init();
	auto obj = CGameObject::FindGameObjectWithTag("GameManager");
	m_pGameManager = obj.lock()->GetComponent<CGameCentipedeMarch>();
	m_pNum.lock()->SetTextWString(L"Time" + std::to_wstring(m_pGameManager.lock()->GetGameTime()));
	m_pNum.lock()->SetColor({0,0,0,1});
	m_pNum.lock()->GetRectTransform()->SetPos(-310,-310);
}
void CTitleSceneManager::Update()
{
	if (MySpace::Input::Keyboad::IsTrigger(VK_W) || MySpace::Input::Keyboad::IsPress(VK_W))
	{
		int time = m_pGameManager.lock()->GetGameTime();
		time += 1;
		m_pNum.lock()->SetTextWString(L"Time" + std::to_wstring(time));
		if (time > 60 * 5)
			time = 60 * 5;
		m_pGameManager.lock()->SetGameTime(time);
	}
	if (MySpace::Input::Keyboad::IsTrigger(VK_S) || MySpace::Input::Keyboad::IsPress(VK_S))
	{
		int time = m_pGameManager.lock()->GetGameTime();
		time -= 1;
		m_pNum.lock()->SetTextWString(L"Time" + std::to_wstring(m_pGameManager.lock()->GetGameTime()));
		if (time < 0)
			time = 1;
		m_pGameManager.lock()->SetGameTime(time);
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

	auto obj = CGameObject::CreateObject().lock();
	m_pPlayer = obj->AddComponent<CCentipede>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CGroundManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CItemManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CEnemyManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CBulletManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CGameUI>();

	auto pObj = CGameObject::FindGameObjectWithTag("GameManager");
	auto game = pObj.lock()->GetComponent<CGameCentipedeMarch>().lock();

	obj = CGameObject::CreateObject().lock();
	m_pTimer = obj->AddComponent<CTimer>();
	m_pTimer.lock()->SetTime((float)game->GetGameTime());
	auto rect = obj->GetComponent<CRectTransform>();
	//rect.lock()->SetPos(CScreen::GetWidth()/2 - CScreen::GetWidth()/5, CScreen::GetHeight()/2 - CScreen::GetHeight()/6);
	rect.lock()->SetPos(0, CScreen::GetHeight() / 2 - CScreen::GetHeight() / 6);
}
void CGameSceneManager::Init()
{
	// �|�C���^�̎擾
	CSceneManagerComponent::Init();
}
void CGameSceneManager::Update()
{
	// �Q�[���I�������Ȃǂ��m�F
	if (!m_pTimer.lock())
		return;

	// �^�C���I�[�o�[
	if (m_pTimer.lock()->GetTime() <= 0.0f)
	{
		m_pFade.lock()->StartFadeOut();
		m_bEnd = true;
	}
	if (!m_pPlayer.lock())
		return;

	// ��ׂ̈ړ�
	if (m_pPlayer.lock()->GetLength() > m_nSize)
	{
		if (auto cam = CCamera::GetMain()->BaseToDerived<CMyCamera>(); cam)
		{
			cam->SetOffset(cam->GetOffset() + 20);
		}
		m_nSize = m_pPlayer.lock()->GetLength() + 5;
	}
	// �̗͊m�F
	if (m_pPlayer.lock()->GetLife() < 0)
	{
		m_pFade.lock()->StartFadeOut();
		m_bEnd = true;
	}
	// �I������
	if (m_bEnd)
	{
		auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
		auto mgr = mgrO.lock()->GetComponent<CGameCentipedeMarch>();
		mgr.lock()->SetResultNum(m_pPlayer.lock()->GetLength());
		m_pPlayer.reset();
	}
}
//=======================================================
void CResultSceneManager::Awake()
{
	GetOwner()->SetName("ResultSceneManager");
	auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
	auto mgr = mgrO.lock()->GetComponent<CGameCentipedeMarch>();
		
	auto obj = CGameObject::CreateObject().lock();
	auto com = obj->AddComponent<CResultUI>();
	com->SetNum(mgr.lock()->GetResultNum());
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

	auto obj = CGameObject::CreateObject().lock();
	m_pPlayer = obj->AddComponent<CCentipede>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CGroundManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CItemManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CEnemyManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CBulletManager>();

	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CGameUI>();
	
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
		obj = CGameObject::CreateObject().lock();
		auto text = obj->AddComponent<CTextRenderer>();
		text->SetTextWString(Text[cnt]);
		text->SetColor({ 0, 0, 0, 1 });
		text->GetRectTransform()->SetPos(pos);
		pos.y -= 100;
	}

	obj = CGameObject::CreateObject().lock();
	m_pTimer = obj->AddComponent<CTimer>();
	m_pTimer.lock()->SetTime(120);
	auto rect = obj->GetComponent<CRectTransform>();
	//rect.lock()->SetPos(CScreen::GetWidth()/2 - CScreen::GetWidth()/5, CScreen::GetHeight()/2 - CScreen::GetHeight()/6);
	rect.lock()->SetPos(0, CScreen::GetHeight() / 2 - CScreen::GetHeight() / 7);
}
void CTutrialSceneManager::Init()
{
	// �|�C���^�̎擾
	CSceneManagerComponent::Init();
}
void CTutrialSceneManager::Update()
{
	// �Q�[���I�������Ȃǂ��m�F
	if (!m_pTimer.lock())
		return;

	if (MySpace::Input::Keyboad::IsTrigger(VK_SPACE))
	{
		m_pFade.lock()->StartFadeOut();
		CSound::Play(eSE::SE_ENTER);
	}

	// �^�C���I�[�o�[
	if (m_pTimer.lock()->GetTime() <= 0.0f)
	{
		m_pFade.lock()->StartFadeOut();
		m_bEnd = true;
	}
	if (!m_pPlayer.lock())
		return;

	// ��ׂ̈ړ�
	if (m_pPlayer.lock()->GetLength() > m_nSize)
	{
		if (auto cam = CCamera::GetMain()->BaseToDerived<CMyCamera>(); cam)
		{
			cam->SetOffset(cam->GetOffset() + 15);
		}
		m_nSize = m_pPlayer.lock()->GetLength() + 5;
	}
	// �̗͊m�F
	//if (m_pPlayer.lock()->GetLife() < 0)
	//{
	//	m_pFade.lock()->StartFadeOut();
	//	m_bEnd = true;
	//}
	// �I������
	if (m_bEnd)
	{
		auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
		auto mgr = mgrO.lock()->GetComponent<CGameCentipedeMarch>();
		mgr.lock()->SetResultNum(m_pPlayer.lock()->GetLength());
		m_pPlayer.reset();
	}
}
