//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 基底クラス
//=========================================================

//--- インクルード部
#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Camera/camera.h>
//#include "collision.h"

//using MyLibrary::System::CGameObjectManager;
using namespace MySpace::SceneManager;
using namespace MySpace::Game;

// コンストラクタ
CScene::CScene()
	:m_SceneName("none")
{
	m_drawManager = std::make_shared<CDrawManager>();
	m_objeManager = std::make_shared<CGameObjectManager>(m_spPtr.lock());

	//CreateEmptyScene();
}
// コンストラクタ
CScene::CScene(std::string name)
	:m_SceneName(name)
{
	m_drawManager = std::make_shared<CDrawManager>();
	m_objeManager = std::make_shared<CGameObjectManager>(m_spPtr.lock());

	//m_objeManager->Awake();
}
// デストラクタ
CScene::~CScene()
{
	//Uninit();

	m_objeManager.reset();
	m_drawManager.reset();
}
// 初期化
void CScene::Init()
{
	// シーンに対してゲームオブジェクトを引き渡す
	//m_objeManager->Init();
}
// 解放
void CScene::Uninit()
{
	// シーンを破棄した際、スマートポインタで破棄される
	m_objeManager->Uninit();
}
// 更新
void CScene::Update()
{
	m_objeManager->Update();
}
// 一定時間の更新
void CScene::FixedUpdate()
{
	m_objeManager->FixedUpdate();
}
// 描画
void CScene::Draw()
{
	m_drawManager->Update();
}
void CScene::CreateEmptyScene()
{
	if (m_objeManager)
	{
		m_objeManager->Uninit();
		m_objeManager.reset();
	}
	m_objeManager = std::make_shared<CGameObjectManager>();
	m_objeManager->CreateBasicObject();
	//m_objeManager->Init();

	m_SceneName = "empty";
}