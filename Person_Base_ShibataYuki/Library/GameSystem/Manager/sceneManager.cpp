//=========================================================
// �쐬:2022/04/20
// �V�[���Ǘ��׽
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <DebugSystem/imguiManager.h>

#include <GameSystem/components.h>

#include <CoreSystem/File/cerealize.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;

#define SCENE_PATH	FORDER_DIR(data/scene/)

// �R���X�g���N�^
CSceneManager::CSceneManager()
{
	m_currentPath = SCENE_PATH + std::string("Title.scene");
	m_sceneDetection = std::make_shared<CSceneTransitionDetection>();
}
void CSceneManager::Init()
{
	// �֐��ݒ�
	SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
	//m_pCurrentScene.lock()->Init();
}
void CSceneManager::Uninit()
{
	for (auto & scene : m_pScenes)
	{
		scene->GetObjManager()->AllUninit();
		scene->Uninit();
		scene.reset();
	}
	m_pScenes.clear();
}
void CSceneManager::Update()
{
	//for (SceneList::iterator scene = m_pScenes.begin(); scene != m_pScenes.end(); ++scene)
	for (auto & scene : m_pScenes)
	{
		
		scene->Update();
	}
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
}
std::weak_ptr<CScene> CSceneManager::SceneTransition(std::string name)
{
	std::shared_ptr<CScene> pNextScene = NewScene(name);
	// obj�̈����n��
	std::vector<std::weak_ptr<CRenderer>> list;
	if (m_pCurrentScene.lock())
	{
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
		// �j�����ꂽ
		if (auto mgr = m_pCurrentScene.lock()->GetDrawManager(); mgr)
			list = mgr->GetList();
	}
	RemoveScene(m_pCurrentScene.lock(), pNextScene);
	pNextScene->GetDrawManager()->DrawPass(list);

	return m_pCurrentScene;
}
std::weak_ptr<CScene> CSceneManager::SetActiveScene(std::shared_ptr<CScene> pNextScene)
{
	// obj�̈����n��
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	auto list = m_pCurrentScene.lock()->GetDrawManager()->GetList();
	RemoveScene(m_pCurrentScene.lock(), pNextScene);
	pNextScene->GetDrawManager()->DrawPass(list);
	return m_pCurrentScene;
}
void CSceneManager::RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext)
{
	// ���C���̐ؑ�
	if (pNext)
	{
		m_pCurrentScene = pNext;
		m_sceneDetection->Call(pRemove.get(), pNext.get());	// �֐��|�C���^�Ăяo��
		AddSceneList(pNext);
	}
	// �z�񂩂�̏��O
	if (auto it = std::find(m_pScenes.begin(), m_pScenes.end(), pRemove); it != m_pScenes.end())
	{
		m_sceneDetection->Call(pRemove.get());	// �j��
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
	// �t�H���_
	//std::string filePathName = "data" + std::string("/") + "scene";
	/*if (_mkdir(filePathName.c_str()) != 0)
	{
		return;
	}*/
	auto scene = m_pCurrentScene.lock();

	// �t�@�C���g���q
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
		// �ۑ��p�̼���ް����쐬
		std::shared_ptr<CSceneData> saveData = std::make_shared<CSceneData>();
		saveData->m_resource.Save();						// Resource���i�[
		saveData->m_SceneName = scene->GetSceneName();		// ���O�i�[
		auto objs = scene->GetObjManager()->GetList();		// �I�u�W�F�N�g���i�[
		for (auto & obj : objs)
		{
			if (obj->GetTransform()->GetParent().lock())continue;	// �e�������
			saveData->m_GameObjectManager.emplace_back(obj);
		}
		// �V���A���C�Y
		sirial.OutputFile(filename, filePathName, saveData);
	}
}
bool CSceneManager::LoadScene(std::string path)
{
	// �Z�[�u���󂯎��p�ϐ�
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

	// �J�����p�X��ۑ�
	m_currentPath = path;

	// �V�����V�[�����쐬
	std::shared_ptr<CScene> newScene = std::make_shared<CScene>(sceneData->m_SceneName);
	RemoveScene(m_pCurrentScene.lock(), newScene);
	if (newScene != m_pCurrentScene.lock()) { 
	
	}

	// �ǂݍ��݂Ƒ��
	sceneData->m_resource.Load();													// ���[�h����Resource��ǂݍ���
	newScene->SetSceneName(sceneData->m_SceneName);							// ���O�ݒ�
	newScene->GetObjManager()->SetObjList(sceneData->m_GameObjectManager);	// �I�u�W�F�N�g�ݒ�

	// ���[�h������
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
		if (ImGui::BeginMenu("Scene"))
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
	if (ImGui::Button("new scene"))
	{
		CScene* newScene = SceneTransition("empty").lock().get();
		newScene->CreateEmptyScene();
	}

	ImGui::InputText("�V�[���J�ږ�", sceneName, 256);
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
