//=========================================================
//�쐬:2022/06/17
//---------------------------------------------------------
// TODO: �V�[���ؑ֎��̏��������K�{
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//--- �C���N���[�h��
#include <CoreSystem/system.h>
#include <CoreSystem/Util/cerealCommon.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/instantResourceManager.h>
#include <GameSystem/Scene/sceneTransitionDetection.h>
//
#include <DebugSystem/debug.h>

namespace MySpace
{
	namespace Game 
	{
		class CCollisionSystem;
		class CDrawSystem;
	}
}

namespace AI
{
	class CNavMeshBake;
}

namespace MySpace
{
	namespace SceneManager
	{
		using namespace MySpace::Game;
		using namespace MySpace::System;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CComponent;
		using MySpace::Game::CInstantResourceManager;
		using MySpace::Game::CCollisionSystem;
		using AI::CNavMeshBake;
		
		//--- �N���X��`
		class CSceneManager : public CSingleton<CSceneManager>
		{
			friend class CSingleton<CSceneManager>;
		private:
			// �V���A���C�Y�p�׽
#pragma region Cereal
			// �ꎞ�I�ɑޔ�������Memento�p�^�[��
			class CSceneData
			{
			private:
				friend cereal::access;
				template <typename Archive>
				void save(Archive & archive, std::uint32_t const version) const
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
				}
				template <typename Archive>
				void load(Archive & archive, std::uint32_t const version)
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
				}
			public:
				std::string m_SceneName;
				std::list<std::shared_ptr<CGameObject> > m_aGameObjectManager;
				CInstantResourceManager m_resource;
			};
#pragma endregion

			//--- �G�C���A�X
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- �����o�ϐ�	
			std::weak_ptr<CScene> m_pCurrentScene;
			SceneList m_aScenes;
			std::string m_currentPath;
			std::shared_ptr<CSceneTransitionDetection> m_sceneDetection;

			std::shared_ptr<CCollisionSystem> m_pCollisionSystem;
			std::shared_ptr<CDrawSystem> m_pDrawSystem;
			std::shared_ptr<CNavMeshBake> m_pNavMesh;
			bool m_bTransition;

		private:
			//--- ���ފ֐�
			CSceneManager();

			// *@�V�[���j�����ɌĂяo�����߂̊֐�
			// *@m_bTransition��true
			// *@�ċA�Ăяo�������
			void* FlagOn(CScene* scene)
			{
				m_bTransition = true;
				CSceneManager::Get().SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
					//std::bind(&CSceneManager::FlagOn, this, std::placeholders::_1));
				return nullptr;
			}
		public:
			void Init();
			void Uninit();
			void Update();
			void FixedUpdateScene();
			void DrawScene();

			// *@CGameObjectManager�p
			// *@component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
			// *@CSceneManager��CScene��CGameObjectManager(�������Ń|�C���^�^���������悩����?��������static)
			bool Escape()
			{
				if (m_bTransition) { m_bTransition = false; return true; }
				return false;
			}

			// *@���C���V�[���擾(�����������SP�擾)
			CScene* GetActiveScene() { return m_pCurrentScene.lock().get(); }
			// *@���C���V�[���擾
			std::shared_ptr<CScene> GetActiveScene(int) { return m_pCurrentScene.lock(); }
			// *@���C���V�[�����O�擾
			std::string GetActiveSceneName() { return m_pCurrentScene.lock()->GetSceneName(); }
			// *@�����񂪈�v����V�[���擾
			std::shared_ptr<CScene> GetSceneByName(std::string name) 
			{
				for (auto & scene : m_aScenes)
				{
					if (scene->GetSceneName() == name) 
						return scene;
				}
				return std::shared_ptr<CScene>();
			}
			// *@�S�V�[���̎擾
			std::vector<std::shared_ptr<CScene>> GetAllScene() { return m_aScenes; }
			
			//--- �V�[���ǂݍ��݊֘A
			// *@�ʏ�̃V�[���ؑ�
			std::weak_ptr<CScene> SceneTransition(std::string name);
			// *@���C���V�[���ؑ�
			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *@�V�[������
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}
			
			// *@�V�[������
			std::shared_ptr<CScene> NewScene(std::string name = std::string());

			// *@���݃V�[���ɒǉ�
			template <class T>
			CScene* AddScene()
			{
				std::shared_ptr<CScene> sp = NewScene();
				if (auto scene = std::dynamic_pointer_cast<T>(sp); !scene)
					return nullptr;
				else {
					AddSceneList(scene);
					return scene.get();
				}
				AddSceneList(sp);
				//sp->StartUp();
				return sp.get();
			}

			// *@�V�[���j��
			// *@�������w��Őؑ�
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		
			// *@���X�g�ɒǉ�
			// *@�i�[�ł����false,���ɑ��݂����true
			bool AddSceneList(std::shared_ptr<CScene> add)
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

			// *@�Z�[�u
			void SaveScene(const std::string filename = "none");
			// *@���[�h
			bool LoadScene(const std::string path);

			//--- �V�[���J�ڎ��o�^�֐�
			// *@�V�[���J�ڎ��Ăяo���o�^
			// *@�߂�l:void
			// *@������:CScene* prev,CScene* next
			template <class T>
			void SceneChanged(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_sceneDetection->Changed<T>(func, ptr);
			}
			// *@�V�[���������Ăяo���o�^
			// *@�߂�l:void
			// *@������:CScene* scene, int mode
			template <class T>
			void SceneLoaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_sceneDetection->Loaded<T>(func, ptr);
			}
			// *@�V�[���j�����Ăяo���o�^
			// *@�߂�l:void
			// *@������:CScene* scene
			template <class T>
			void SceneUnloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_sceneDetection->Unloaded<T>(func, ptr);
			}

			// *@�ؑ֌Ăяo���׽�̎擾
			inline CSceneTransitionDetection* GetDetection() { return m_sceneDetection.get(); }
			
			// *@�R���W�����V�X�e���̎擾
			inline std::shared_ptr<CCollisionSystem> GetCollisionSystem() { return m_pCollisionSystem; }
			
			// *@�`��V�X�e���̎擾
			inline std::shared_ptr<CDrawSystem> GetDrawSystem() { return m_pDrawSystem; }

			// *@����NavMesh�̎擾
			inline std::shared_ptr<CNavMeshBake> GetNavMesh() { return m_pNavMesh; };

#ifdef BUILD_MODE
		private:
			char m_cDebugSceneName[256] = "none";
		public:
			void ImGuiDebug();

#endif // BUILED_MODE
		};
	}
}
using namespace MySpace::SceneManager;


#endif // !__GAME_SCENE_H__