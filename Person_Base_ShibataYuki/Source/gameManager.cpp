//========================================================
// [gameManager.h]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

//--- �C���N���[�h��
#include <gameManager.h>
#include <fadeController.h>
#include <gameSceneManager.h>
#include <titleSceneManager.h>
#include <spellDefine.h>

#include <GameSystem/Manager/sceneManager.h>

using namespace Spell;

//========================================================
// �R���X�g���N�^
//========================================================
CGameManager::CGameManager()
	:m_pGameScene(nullptr)
{
}

//========================================================
// �������t���R���X�g���N�^
//========================================================
CGameManager::CGameManager(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// �f�X�g���N�^
//========================================================
CGameManager::~CGameManager()
{
}

//========================================================
// �������Ăяo��
//========================================================
void CGameManager::Awake()
{	
	// ��j��o�^
	CGameObject::DontDestroy(GetOwner(0));

	// �֐��|�C���^�ݒ�
	MySpace::SceneManager::CSceneManager::Get().SceneChanged<Spell::CGameManager>(&CGameManager::SceneResponce, this);

}

//========================================================
// ������
//========================================================
void CGameManager::Init()
{
	{
		auto obj = CGameObject::CreateObject().lock();
		m_pFade = obj->AddComponent<CFadeController>();
		obj->SetObjTag(Spell::TAG_FADE);

		// ��j��o�^
		CGameObject::DontDestroy(obj);
	}

	{
		auto obj = CGameObject::CreateObject().lock();
		auto title = obj->AddComponent<Spell::CTitleSceneManager>();
		title->Create();
	}
}

//========================================================
// �X�V
//========================================================
void CGameManager::Update()
{

}

//========================================================
// Scene�ؑ֎��Ăяo���p
//========================================================
void* CGameManager::SceneResponce(MySpace::SceneManager::CScene* prev, MySpace::SceneManager::CScene* next)
{
	next->CreateEmptyScene();

	if (prev->GetSceneName() == "")
	{

	}

	if (next->GetSceneName() == Spell::OBJ_NAME_TITLE)
	{
		auto obj = CGameObject::CreateObject().lock();
		obj->AddComponent<Spell::CTitleSceneManager>();
	}

	if (next->GetSceneName() == Spell::OBJ_NAME_GAME)
	{
		auto obj = CGameObject::CreateObject().lock();
		obj->AddComponent<Spell::CGameSceneManager>();
	}

	// �ēx�֐��|�C���^�ݒ�
	MySpace::SceneManager::CSceneManager::Get().SceneChanged<Spell::CGameManager>(&CGameManager::SceneResponce, this);

	return nullptr;
}
