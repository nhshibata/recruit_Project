//=========================================================
// 作成:2022/04/20
// シーン管理ｸﾗｽ
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/components.h>
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/Manager/drawSystem.h>
#include <DebugSystem/imguiManager.h>
#include <AISystem/Nav/navMeshBake.h>

#include <CoreSystem/File/cerealize.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;

#define SCENE_PATH	FORDER_DIR(data/scene/)

// コンストラクタ
CSceneManager::CSceneManager()
{
	m_currentPath = SCENE_PATH + std::string("Title.scene");
	m_sceneDetection = std::make_shared<CSceneTransitionDetection>();
	m_pCollisionSystem = std::make_shared<CCollisionSystem>();
	m_pDrawSystem = std::make_shared<CDrawSystem>();
	m_pNavMesh = std::make_shared<CNavMeshBake>();
}
void CSceneManager::Init()
{
	// 関数設定
	SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
	//m_pCurrentScene.lock()->Init();
	m_pNavMesh->Init();
}
void CSceneManager::Uninit()
{
	for (auto & scene : m_pScenes)
	{
		scene->GetObjManager()->AllUninit();
		scene->Uninit();
		scene.reset();
	}
	m_sceneDetection.reset();
	m_pCollisionSystem.reset();
	m_pDrawSystem.reset();
	m_pScenes.clear();
}
void CSceneManager::Update()
{
	//for (SceneList::iterator scene = m_pScenes.begin(); scene != m_pScenes.end(); ++scene)
	for (auto & scene : m_pScenes)
	{
		scene->Update();
	}

	// 当たり判定の確認
	m_pCollisionSystem->CollisionCheck();
}
void CSceneManager::FixedUpdate()
{
	for (auto & scene : m_pScenes)
	{
		scene->FixedUpdate();
	}
}
void CSceneManager::Draw()
{
	for (auto & scene : m_pScenes)
	{
		scene->Draw();
	}

	m_pDrawSystem->Update();
	m_pNavMesh->Draw();
}
// シーン切替
std::weak_ptr<CScene> CSceneManager::SceneTransition(std::string name)
{
	std::shared_ptr<CScene> pNextScene = NewScene(name);
	// objの引き渡し
	if (m_pCurrentScene.lock())
	{
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	}
	RemoveScene(m_pCurrentScene.lock(), pNextScene);

	return m_pCurrentScene;
}
// メインシーンの切り替え
std::weak_ptr<CScene> CSceneManager::SetActiveScene(std::shared_ptr<CScene> pNextScene)
{
	// objの引き渡し
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	RemoveScene(m_pCurrentScene.lock(), pNextScene);
	return m_pCurrentScene;
}
// シーン生成
std::shared_ptr<CScene> CSceneManager::NewScene(std::string name)
{
	std::shared_ptr<CScene> pNextScene;

	// 名前指定なし
	if (name.empty())
		pNextScene = std::make_shared<CScene>();
	else
		pNextScene = std::make_shared<CScene>(name);
	// リストへの追加
	AddSceneList(pNextScene);
	// 初期化:自分のSPを教える
	pNextScene->Init(pNextScene);

	if (!m_pCurrentScene.lock())
		m_pCurrentScene = pNextScene;
	m_sceneDetection->Call(pNextScene.get(), 0);	// 生成時呼び出し
	return pNextScene;
}
// シーン破棄
void CSceneManager::RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext)
{
	// メインの切替
	if (pNext)
	{
		m_pCurrentScene = pNext;
		m_sceneDetection->Call(pRemove.get(), pNext.get());	// 関数ポインタ呼び出し
		AddSceneList(pNext);
	}
	// 配列からの除外
	if (auto it = std::find(m_pScenes.begin(), m_pScenes.end(), pRemove); it != m_pScenes.end())
	{
		m_sceneDetection->Call(pRemove.get());	// 破棄
		pRemove.reset();
		m_pScenes.erase(it);
	}
	if (!m_pCurrentScene.lock() && !pNext)
	{
		m_pCurrentScene = m_pScenes.front();
	}
}
void CSceneManager::SaveScene(const std::string filename)
{
	// フォルダ
	//std::string filePathName = "data" + std::string("/") + "scene";
	/*if (_mkdir(filePathName.c_str()) != 0)
	{
		return;
	}*/
	auto scene = m_pCurrentScene.lock();

	// ファイル拡張子
	std::string name;
	if (filename == "none")
	{
		name = scene->GetSceneName() + ".scene";
	}
	else
	{
		name = filename.c_str();
		name += ".scene";
	}
	
	//std::string filePathName = "data/" + filePathName + "/" + name;
	std::string filePathName = SCENE_PATH + name;
	{
		CCerealize<std::shared_ptr<CSceneData>> sirial;
		// 保存用のｼｰﾝﾃﾞｰﾀを作成
		std::shared_ptr<CSceneData> saveData = std::make_shared<CSceneData>();
		saveData->m_resource.Save();						// Resourceを格納
		saveData->m_SceneName = scene->GetSceneName();		// 名前格納
		auto objs = scene->GetObjManager()->GetList();		// オブジェクトを格納
		for (auto & obj : objs)
		{
			if (obj->GetTransform()->GetParent().lock())continue;	// 親が居れば
			saveData->m_GameObjectManager.emplace_back(obj);
		}
		// シリアライズ
		sirial.OutputFile(filename, filePathName, saveData);
	}
}
bool CSceneManager::LoadScene(std::string path)
{
	// セーブ情報受け取り用変数
	std::shared_ptr<CSceneData> sceneData = std::make_shared<CSceneData>();
	/*std::ifstream ifs(path, std::ios::in);

	if (!ifs)return false;
	{
		cereal::JSONInputArchive archive(ifs);
		archive(sceneData);
	}
	ifs.close();*/
	{
		CCerealize<std::shared_ptr<CSceneData>> sirial;
		sceneData = sirial.InputFile(path);
	}
	if (!sceneData)return false;

	// 開いたパスを保存
	m_currentPath = path;

	// 新しいシーンを作成
	std::shared_ptr<CScene> newScene = std::make_shared<CScene>(sceneData->m_SceneName);
	RemoveScene(m_pCurrentScene.lock(), newScene);
	if (newScene != m_pCurrentScene.lock()) { 
	
	}

	// 読み込みと代入
	sceneData->m_resource.Load();													// ロードしたResourceを読み込み
	newScene->SetSceneName(sceneData->m_SceneName);							// 名前設定
	newScene->GetObjManager()->SetObjList(sceneData->m_GameObjectManager);	// オブジェクト設定

	// ロード時処理
	for (auto & obj : sceneData->m_GameObjectManager)
	{
		obj->OnLoad();
	}

	return true;
}

#ifdef BUILD_MODE

void CSceneManager::ImguiDebug()
{
	static char sceneName[256] = "none";

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"Scene"))
		{
			for (SceneList::iterator it = m_pScenes.begin(); it != m_pScenes.end(); ++it)
			{
				if (ImGui::MenuItem((*it)->GetSceneName().c_str()))
				{
					m_pCurrentScene = (*it);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::Button(u8"new scene"))
	{
		CScene* newScene = SceneTransition("empty").lock().get();
		newScene->CreateEmptyScene();
	}

	ImGui::InputText("u8シーン遷移名", sceneName, 256);
	ImGui::SameLine();
	if (ImGui::Button("change scene"))
	{
		auto i = SceneTransition(sceneName).lock().get();
	}
	//if (ImGui::Button("add scene"))
	//{
	//	CScene* newScene = AddScene<CScene>();
	//}
}

#endif // BUILD_MODE
