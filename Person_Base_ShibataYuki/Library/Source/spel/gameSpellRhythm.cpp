//=========================================================
// [gameSpellRhythm.cpp] 
// �쐬:2022/09/20
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <gameSpellRhythm.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <CoreSystem/Input/Keyboard.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>

// �Q�[��
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
	m_pGameCamera = obj->AddComponent<Spell::CGameCamera>();
	//CGameObject::DontDestroy(obj);
	
	m_pFade.lock()->SetTypeFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_OUT_START, &CGameSpellRhythm::FadeFuncPtr, this);
	/*m_pFade.lock()->SetFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_OUT_START, FadeFuncPtr, this, true);
	m_pFade.lock()->SetFunction(CFadeController::E_FadeState::FADE_IN_START, FadeFuncPtr, true);*/
}
void CGameSpellRhythm::Init()  
{
	// �֐��̃Z�b�g
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
	// ��אؑ�
	// debugCamera�̕��Őؑւ���������ׂ���?
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
	// fade in/out �J�n���ɂ͒�~��Ԃɂ���
	if (m_pFade.lock()->GetState() == CFadeController::E_FadeState::FADE_IN)
		state = CGameObject::E_ObjectState::ACTIVE;

	ObjStateSwitch(state, MySpace::SceneManager::CSceneManager::Get().GetActiveScene());
}
// ��~/�s����ԂɕύX
void CGameSpellRhythm::ObjStateSwitch(CGameObject::E_ObjectState state, CScene* scene)
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
// �V�[���J�ڌĂяo��
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

	// �Ǘ��I�u�W�F�N�g�𐶐�
	// �Q�[��
	if (next->GetSceneName() == "Game")
	{
		auto obj = next->GetObjManager()->CreateGameObject();
		obj->AddComponent<CGameSceneManager>();

		// �f�o�b�O���
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();

		// ���C����אݒ�
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

		// �f�o�b�O���
		obj = CGameObject::CreateObject().lock();
		m_pDebugCamera = obj->AddComponent<CDebugCamera>();

		// ���C����אݒ�
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
		if (auto com = m_pGameCamera.lock()->GetComponent<CModelRenderer>(); com)
			com->SetActive(false);
		CSound::Play(eBGM::BGM_RESULT);
	}

	// ���̃V�[����obj���~��Ԃ�
	ObjStateSwitch(CGameObject::E_ObjectState::WAIT, next);
	// �t�F�[�h�C���I����
	m_pFade.lock()->SetTypeFunction<CGameSpellRhythm>(CFadeController::E_FadeState::FADE_IN, &CGameSpellRhythm::FadeFuncPtr, this);
	m_pFade.lock()->StartFadeIn();

	// �ċA
	CSceneManager::Get().SceneChanged<CGameSpellRhythm>(&CGameSpellRhythm::CreateManager, this);
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
	//obj->AddComponent<CTitleUI>();

	//obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CTextRenderer>();

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
	// �|�C���^�̎擾
	CSceneManagerComponent::Init();
}
void CGameSceneManager::Update()
{
	// �I������
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
	// �|�C���^�̎擾
	CSceneManagerComponent::Init();
}
void CTutrialSceneManager::Update()
{
	// �Q�[���I�������Ȃǂ��m�F

	if (MySpace::Input::Keyboad::IsTrigger(VK_SPACE))
	{
		m_pFade.lock()->StartFadeOut();
		CSound::Play(eSE::SE_ENTER);
	}

	
	// �I������
	if (m_bEnd)
	{
		auto mgrO = CGameObject::FindGameObjectWithTag("GameManager");
		auto mgr = mgrO.lock()->GetComponent<CGameSpellRhythm>();
	}
}
