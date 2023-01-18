//========================================================
// [gameManager.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

//--- インクルード部
#include <gameManager.h>
#include <fadeController.h>
#include <gameSceneManager.h>
#include <titleSceneManager.h>
#include <spellDefine.h>

#include <GameSystem/Manager/sceneManager.h>

using namespace Spell;

//========================================================
// コンストラクタ
//========================================================
CGameManager::CGameManager()
	:m_pGameScene(nullptr)
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CGameManager::CGameManager(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// デストラクタ
//========================================================
CGameManager::~CGameManager()
{
}

//========================================================
// 生成時呼び出し
//========================================================
void CGameManager::Awake()
{	
	// 非破壊登録
	CGameObject::DontDestroy(GetOwner(0));

	// 関数ポインタ設定
	MySpace::SceneManager::CSceneManager::Get().SceneChanged<Spell::CGameManager>(&CGameManager::SceneResponce, this);

}

//========================================================
// 初期化
//========================================================
void CGameManager::Init()
{
	{
		auto obj = CGameObject::CreateObject().lock();
		m_pFade = obj->AddComponent<CFadeController>();
		obj->SetObjTag(Spell::TAG_FADE);

		// 非破壊登録
		CGameObject::DontDestroy(obj);
	}

	{
		auto obj = CGameObject::CreateObject().lock();
		auto title = obj->AddComponent<Spell::CTitleSceneManager>();
		title->Create();
	}
}

//========================================================
// 更新
//========================================================
void CGameManager::Update()
{

}

//========================================================
// Scene切替時呼び出し用
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

	// 再度関数ポインタ設定
	MySpace::SceneManager::CSceneManager::Get().SceneChanged<Spell::CGameManager>(&CGameManager::SceneResponce, this);

	return nullptr;
}
