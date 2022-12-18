//=========================================================
// �쐬:2022/04/20
// �V�[���Ǘ��׽
// 
//=========================================================

//--- �C���N���[�h��
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

// �R���X�g���N�^
CSceneManager::CSceneManager()
	:m_bTransition(false), m_currentPath(std::string())
{
	m_sceneDetection.reset();
	m_pCollisionSystem.reset();
	m_pDrawSystem.reset();
	m_pNavMesh.reset();

	m_sceneDetection = std::make_shared<CSceneTransitionDetection>();
	m_pCollisionSystem = std::make_shared<CCollisionSystem>();
	m_pDrawSystem = std::make_shared<CDrawSystem>();
	m_pNavMesh = std::make_shared<CNavMeshBake>();
}
void CSceneManager::Init()
{
	m_currentPath = SCENE_PATH + std::string("Title.scene");
	// �֐��ݒ�
	SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
	//m_pCurrentScene.lock()->Init();
	m_pNavMesh->Init();
}
void CSceneManager::Uninit()
{
	for (auto & scene : m_aScenes)
	{
		scene->GetObjManager()->AllUninit();
		scene->Uninit();
		scene.reset();
	}
	m_sceneDetection.reset();
	m_pCollisionSystem.reset();
	m_pDrawSystem.reset();
	m_aScenes.clear();
}
void CSceneManager::UpdateScene()
{
	//for (SceneList::iterator scene = m_pScenes.begin(); scene != m_pScenes.end(); ++scene)
	for (auto & scene : m_aScenes)
	{
		scene->Update();
	}

	// �����蔻��̊m�F
	m_pCollisionSystem->CollisionCheck();
}
void CSceneManager::FixedUpdateScene()
{
	for (auto & scene : m_aScenes)
	{
		scene->FixedUpdate();
	}
}
void CSceneManager::DrawScene()
{
	for (auto & scene : m_aScenes)
	{
		scene->Draw();
	}

	m_pDrawSystem->Update();
	m_pNavMesh->Draw();
}
// �V�[���ؑ�
std::weak_ptr<CScene> CSceneManager::SceneTransition(std::string name)
{
	std::shared_ptr<CScene> pNextScene = NewScene(name);
	// obj�̈����n��
	if (m_pCurrentScene.lock())
	{
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	}
	RemoveScene(m_pCurrentScene.lock(), pNextScene);

	return m_pCurrentScene;
}
// ���C���V�[���̐؂�ւ�
std::weak_ptr<CScene> CSceneManager::SetActiveScene(std::shared_ptr<CScene> pNextScene)
{
	// obj�̈����n��
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	RemoveScene(m_pCurrentScene.lock(), pNextScene);
	return m_pCurrentScene;
}
// �V�[������
std::shared_ptr<CScene> CSceneManager::NewScene(std::string name)
{
	std::shared_ptr<CScene> pNextScene;

	// ���O�w��Ȃ�
	if (name.empty())
		pNextScene = std::make_shared<CScene>();
	else
		pNextScene = std::make_shared<CScene>(name);
	// ���X�g�ւ̒ǉ�
	AddSceneList(pNextScene);
	// ������:������SP��������
	pNextScene->Init(pNextScene);

	if (!m_pCurrentScene.lock())
		m_pCurrentScene = pNextScene;
	m_sceneDetection->Call(pNextScene.get(), 0);	// �������Ăяo��
	return pNextScene;
}
// �V�[���j��
void CSceneManager::RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext)
{
	//--- ���C���̐ؑ�
	if (pNext)
	{
		m_pCurrentScene = pNext;
		m_sceneDetection->Call(pRemove.get(), pNext.get());	// �֐��|�C���^�Ăяo��
		AddSceneList(pNext);
	}
	//--- �z�񂩂�̏��O
	if (auto it = std::find(m_aScenes.begin(), m_aScenes.end(), pRemove); it != m_aScenes.end())
	{
		m_sceneDetection->Call(pRemove.get());	// �j���Ăяo��
		pRemove->Uninit();						// ���
		{
			CInstantResourceManager instant;
			instant.SceneUnload();
		}
		pRemove.reset();
		m_aScenes.erase(it);
	}
	//--- �A�N�e�B�u�V�[�����Ȃ���ΐݒ�
	if (!m_pCurrentScene.lock() && !pNext)
	{
		m_pCurrentScene = m_aScenes.front();
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
			saveData->m_aGameObjectManager.emplace_back(obj);
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
	sceneData->m_resource.Load();											// ���[�h����Resource��ǂݍ���
	newScene->SetSceneName(sceneData->m_SceneName);							// ���O�ݒ�
	newScene->GetObjManager()->SetObjList(sceneData->m_aGameObjectManager);	// �I�u�W�F�N�g�ݒ�

	// ���[�h������
	for (auto & obj : sceneData->m_aGameObjectManager)
	{
		obj->OnLoad();
	}

	return true;
}

#ifdef BUILD_MODE

void CSceneManager::ImGuiDebug()
{
	//--- ���\��
	if (!ImGui::TreeNode("--- Scene ---"))
		return;

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"Scene"))
		{
			for (SceneList::iterator it = m_aScenes.begin(); it != m_aScenes.end(); ++it)
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

	ImGui::InputText(u8"�V�[���J�ږ�", m_cDebugSceneName, 256);
	ImGui::SameLine();
	if (ImGui::Button("change scene"))
	{
		auto i = SceneTransition(m_cDebugSceneName).lock().get();
	}
	//if (ImGui::Button("add scene"))
	//{
	//	CScene* newScene = AddScene<CScene>();
	//}

	ImGui::TreePop();
}

#endif // BUILD_MODE
