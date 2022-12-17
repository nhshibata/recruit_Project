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
	//CreateEmptyScene();
}
// コンストラクタ
CScene::CScene(std::string name)
	:m_SceneName(name)
{
	//m_objeManager->Awake();
}
// デストラクタ
CScene::~CScene()
{
	//Uninit();

	m_pObjeManager.reset();
}
// 初期化
void CScene::Init(std::weak_ptr<CScene> scene)
{
	m_spPtr = scene;
	m_pObjeManager = std::make_shared<CGameObjectManager>(scene.lock());
	// シーンに対してゲームオブジェクトを引き渡す
	//m_objeManager->Init();
}
// 解放
void CScene::Uninit()
{
	// シーンを破棄した際、スマートポインタで破棄される
	m_pObjeManager->Uninit();
}
// 更新
void CScene::Update()
{
	m_pObjeManager->Update();
}
// 一定時間の更新
void CScene::FixedUpdate()
{
	m_pObjeManager->FixedUpdate();
}
// 描画
void CScene::Draw()
{
}
void CScene::CreateEmptyScene()
{
	if (m_pObjeManager)
	{
		m_pObjeManager->Uninit();
	}
	//m_objeManager = std::make_shared<CGameObjectManager>();
	m_pObjeManager->CreateBasicObject();
	//m_objeManager->Init();
	if(m_SceneName.empty())
		m_SceneName = "empty";
}