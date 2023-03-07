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
#include <AISystem/Nav/navMeshBake.h>

#include <DebugSystem/imguiManager.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;

#define SCENE_PATH	FORDER_DIR(Data/scene/)

//========================================================
// �R���X�g���N�^
//========================================================
CSceneManager::CSceneManager()
	:m_bTransition(false), m_currentPath(std::string()),
	m_sceneDetection(nullptr), m_pCollisionSystem(nullptr), m_pDrawSystem(nullptr),
	m_pNavMesh(nullptr)
{
	m_sceneDetection = new CSceneTransitionDetection();
	m_pCollisionSystem = new CCollisionSystem();
	m_pDrawSystem = new CDrawSystem();
	m_pNavMesh = new CNavMeshBake();
}

//========================================================
// �f�X�g���N�^
//========================================================
CSceneManager::~CSceneManager()
{
	
}

//========================================================
// ������
//========================================================
HRESULT CSceneManager::Init()
{
	HRESULT hr = S_OK;

	// NavMesh
	m_pNavMesh->Init();

	m_pDrawSystem->Init();

	// �p�X�ݒ�
	m_currentPath = SCENE_PATH + std::string("Title.scene");
	
	// �֐��ݒ�
	SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
	
	//--- Scene�J�n
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->Init(m_pCurrentScene);
	else
	{
		//CreateNewScene<CScene>("Title");
		auto newScene = NewScene("Title");
		// Scene���r���Œǉ�����Ă��Ȃ�
		if (!m_pCurrentScene.lock())
			m_pCurrentScene = newScene;
	}

	return hr;
}

//========================================================
// �������
//========================================================
void CSceneManager::Uninit()
{
	for (auto & scene : m_aScenes)
	{
		scene->GetObjManager()->AllUninit();
		scene->Uninit();
		scene.reset();
	}

	delete m_sceneDetection;
	delete m_pCollisionSystem;
	delete m_pDrawSystem;
	delete m_pNavMesh;
	
	m_aScenes.clear();
}

//========================================================
// �V�[���X�V
//========================================================
void CSceneManager::UpdateScene()
{
	for (auto & scene : m_aScenes)
	{
		scene->Update();
	}

	// �����蔻��̊m�F
	m_pCollisionSystem->CollisionCheck();
}

//========================================================
// �Œ莞�ԍX�V
//========================================================
void CSceneManager::FixedUpdateScene()
{
	for (auto & scene : m_aScenes)
	{
		scene->FixedUpdate();
	}
}

//========================================================
// �`��
//========================================================
void CSceneManager::DrawScene()
{
	for (auto & scene : m_aScenes)
	{
		scene->Draw();
	}

	m_pDrawSystem->Update();
	m_pNavMesh->Draw();
}

//========================================================
// ���O����V�[���߲���擾
//========================================================
std::shared_ptr<CScene> CSceneManager::GetSceneByName(std::string name)
{
	for (auto & scene : m_aScenes)
	{
		if (scene->GetSceneName() == name)
			return scene;
	}
	return std::shared_ptr<CScene>();
}

//========================================================
// �V�[���ؑ�
//========================================================
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

//========================================================
// ���C���V�[���̐؂�ւ�
//========================================================
std::weak_ptr<CScene> CSceneManager::SetActiveScene(std::shared_ptr<CScene> pNextScene)
{
	// obj�̈����n��
	if (m_pCurrentScene.lock())
		m_pCurrentScene.lock()->GetObjManager()->PassDontDestroyList(pNextScene->GetObjManager());
	RemoveScene(m_pCurrentScene.lock(), pNextScene);
	return m_pCurrentScene;
}

//========================================================
// �V�[������
//========================================================
std::shared_ptr<CScene> CSceneManager::NewScene(std::string name)
{
	std::shared_ptr<CScene> pNextScene;

	//--- �������m��
	// ���O�w��Ȃ�
	if (name.empty())
		pNextScene = std::make_shared<CScene>();
	else
		pNextScene = std::make_shared<CScene>(name);

	// ���X�g�ւ̒ǉ�
	AddList(pNextScene);

	//--- ������:������SP��������
	pNextScene->Init(pNextScene);

	if (!m_pCurrentScene.lock())
		m_pCurrentScene = pNextScene;
	
	//--- �������Ăяo��
	m_sceneDetection->Call(pNextScene.get(), 0);

	return pNextScene;
}

//========================================================
// �V�[���j��
//========================================================
void CSceneManager::RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext)
{
	//--- ���C���̐ؑ�
	if (pNext)
	{
		m_pCurrentScene = pNext;
		// �֐��|�C���^�Ăяo��
		m_sceneDetection->Call(pRemove.get(), pNext.get());	
		AddList(pNext);
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

//========================================================
// ���X�g�ɒǉ�
//========================================================
bool CSceneManager::AddList(std::shared_ptr<CScene> add)
{
	for (auto & scene : m_aScenes)
	{
		if (scene == add)
		{
			return false;
		}
	}
	m_aScenes.push_back(add);
	return true;
}

//========================================================
// �V�[���ۑ�
//========================================================
void CSceneManager::SaveScene(const std::string filename)
{
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
		if(name.find(".scene") == std::string::npos)
			name += ".scene";
	}
	
	std::string filePathName = SCENE_PATH + name;
	// �p�X�w�肳��Ă��邩
	if(name.find("Data/scene") != std::string::npos)
		filePathName = name;
	{
		CCerealize<std::shared_ptr<CSceneData>> sirial;

		//--- �ۑ��p�̼���ް����쐬
		std::shared_ptr<CSceneData> saveData = std::make_shared<CSceneData>();
		saveData->m_resource.Save();						// Resource���i�[
		saveData->m_SceneName = scene->GetSceneName();		// ���O�i�[
		auto objs = scene->GetObjManager()->GetList();		// �I�u�W�F�N�g���i�[
		for (auto & obj : objs)
		{
			if (obj->GetTransform()->GetParent().lock())continue;	// �e�������
			saveData->m_aGameObjectManager.emplace_back(obj);
			//saveData->m_aGameObjectManager.push_back(obj);
		}
		// �V���A���C�Y
		sirial.OutputFile(filename, filePathName, saveData);
	}
}

//========================================================
// �V�[���ǂݍ���
//========================================================
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
	if (!sceneData)
		return false;

	// �J�����p�X��ۑ�
	m_currentPath = path;

	// �V����Scene���쐬
	std::shared_ptr<CScene> newScene = std::make_shared<CScene>(sceneData->m_SceneName);
	// Scene�ؑ�
	RemoveScene(m_pCurrentScene.lock(), newScene);
	if (newScene != m_pCurrentScene.lock()) { 
	
	}

	// �ǂݍ��݂Ƒ��
	sceneData->m_resource.Load();											// ���[�h����Resource��ǂݍ���
	newScene->Init(newScene);
	newScene->SetSceneName(sceneData->m_SceneName);							// ���O�ݒ�
	// ���[�h������
	for (auto & obj : sceneData->m_aGameObjectManager)
	{
		obj->SetPtr(obj);
		obj->SetScene(newScene);

		auto coms = obj->GetComponentList();
		for (auto & com : coms)
		{
			com->SetOwner(obj);
		}
	}
	newScene->GetObjManager()->SetObjList(sceneData->m_aGameObjectManager);	// �I�u�W�F�N�g�ݒ�

	// ���[�h������
	for (auto & obj : sceneData->m_aGameObjectManager)
	{
		obj->OnLoad();
	}

	return true;
}

//========================================================
// �V���O���g���̎擾
// �ÓI�ϐ�
//========================================================
CSceneManager* CSceneManager::Get()
{
	static CSceneManager pInstance;
	return &pInstance;
}


#ifdef BUILD_MODE

void CSceneManager::ImGuiDebug()
{
	//--- ���\��
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu("Scene"))
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

	if (ImGui::Button("change scene"))
	{
		auto i = SceneTransition(m_cDebugSceneName).lock().get();
	}
	ImGui::SameLine();
	ImGui::InputText("Change Scene", m_cDebugSceneName, 256);

	//if (ImGui::Button("add scene"))
	//{
	//	CScene* newScene = AddScene<CScene>();
	//}

}

#endif // BUILD_MODE
