//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 派生クラス
//=========================================================

//--- インクルード部
#include <GameSystem/Scene/buildScene.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <DebugSystem/imguiManager.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Debug;

// コンストラクタ
CBuildScene::CBuildScene()
	:CScene("Build")
{
	Init();
}
CBuildScene::CBuildScene(std::string name)
	:CScene(name)
{

}
CBuildScene::~CBuildScene()
{
}
void CBuildScene::Init()
{
	// Build構築に必要な関数を呼び出す
	auto obj = m_objeManager->CreateGameObject();
	obj->AddComponent<CDebugCamera>();

	obj = m_objeManager->CreateGameObject();
	obj->AddComponent<CDirectionalLight>();

	CScene::Init();

	// シーン読み込み
	//CSceneManager::Get().LoadScene(FORDER_DIR(Data/scene/build2.scene));

	// ポーズ状態
	//Debug::ImGuiManager::Get().SetPause(true);
	//for(this->GetObjManager()->GetList());
}
void CBuildScene::Uninit()
{
	CScene::Uninit();
}
void CBuildScene::Update()
{
	CScene::Update();
}
void CBuildScene::Draw()
{
	CScene::Draw();
}