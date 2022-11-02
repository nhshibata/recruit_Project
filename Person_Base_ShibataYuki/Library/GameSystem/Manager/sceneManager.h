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
}

namespace MySpace
{
	namespace SceneManager
	{
		using namespace MySpace::Game;
		using namespace MySpace::System;
		using MySpace::System::CAppSystem;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CComponent;
		using MySpace::Game::CInstantResourceManager;
		
		//--- �N���X��`
		class CSceneManager : public CAppSystem<CSceneManager>
		{
			friend class CAppSystem<CSceneManager>;
			friend class CSingleton<CSceneManager>;
		private:
			// �V���A���C�Y�p�׽
			// �ꎞ�I�ɑޔ�������Memento�p�^�[��
			class CSceneData
			{
			private:
				friend cereal::access;
				template <typename Archive>
				void save(Archive & archive, std::uint32_t const version) const
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_GameObjectManager), CEREAL_NVP(m_resource));
				}
				template <typename Archive>
				void load(Archive & archive, std::uint32_t const version)
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_GameObjectManager), CEREAL_NVP(m_resource));
				}
			public:
				std::string m_SceneName;
				std::list<std::shared_ptr<CGameObject> > m_GameObjectManager;
				CInstantResourceManager m_resource;
			};

			// �G�C���A�X
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- �����o�ϐ�	
			std::weak_ptr<CScene> m_pCurrentScene;
			SceneList m_pScenes;
			std::string m_currentPath;
			std::shared_ptr<CSceneTransitionDetection> m_sceneDetection;
			bool m_bTransition;

		private:
			//--- ���ފ֐�
			CSceneManager();

			// *�V�[���j�����ɌĂяo�����߂̊֐�
			// *m_bTransition��true
			// *�ċA�Ăяo�������
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
			void FixedUpdate();
			void Draw();

			// *CGameObjectManager�p
			// *component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
			// *CSceneManager��CScene��CGameObjectManager(�������Ń|�C���^�^���������悩����?��������static)
			bool Escape()
			{
				if (m_bTransition) { m_bTransition = false; return true; }
				return false;
			}

			// *���C���V�[���擾(�����������SP�擾)
			CScene* GetActiveScene() { return m_pCurrentScene.lock().get(); }
			// *���C���V�[���擾
			std::shared_ptr<CScene> GetActiveScene(int) { return m_pCurrentScene.lock(); }
			std::string GetActiveSceneName() { return m_pCurrentScene.lock()->GetSceneName(); }
			// *�����񂪈�v����V�[���擾
			std::shared_ptr<CScene> GetSceneByName(std::string name) 
			{
				for (auto & scene : m_pScenes)
				{
					if (scene->GetSceneName() == name) 
						return scene;
				}
				return std::shared_ptr<CScene>();
			}
			// *�S�V�[���̎擾
			std::vector<std::shared_ptr<CScene>> GetAllScene() { return m_pScenes; }
			
			//--- �V�[���̓ǂݍ���
			// *�ʏ�̃V�[���ؑ�
			std::weak_ptr<CScene> SceneTransition(std::string name);

			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *�V�[������
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}
			
			std::shared_ptr<CScene> NewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene;

				// ���O�w��Ȃ�
				if(name.empty())
					pNextScene = std::make_shared<CScene>();
				else
					pNextScene = std::make_shared<CScene>(name);
				// ���X�g�ւ̒ǉ�
				AddSceneList(pNextScene);
				// ������SP��������
				pNextScene->SetScene(pNextScene);
				
				if (!m_pCurrentScene.lock())
					m_pCurrentScene = pNextScene;
				m_sceneDetection->Call(pNextScene.get(),0);	// �������Ăяo��
				return pNextScene;
			}

			// ���݃V�[���ɒǉ�
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

			// �V�[���j��
			// �������w��Őؑ�
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		
			// *���X�g�ɒǉ�
			// *�i�[�ł����false,���ɑ��݂����true
			bool AddSceneList(std::shared_ptr<CScene> add)
			{
				for (auto & scene : m_pScenes)
				{
					if (scene == add)
					{
						return false;
					}
				}
				m_pScenes.push_back(add);
				return true;
			}

			void SaveScene(const std::string filename = "none");
			bool LoadScene(const std::string path);

			//--- �V�[���J�ڎ��o�^�֐�
			// *�V�[���J�ڎ��Ăяo���o�^
			// *�߂�l:void
			// *������:CScene* prev,CScene* next
			template <class T>
			void SceneChanged(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_sceneDetection->Changed<T>(func, ptr);
			}
			// *�V�[���������Ăяo���o�^
			// *�߂�l:void
			// *������:CScene* scene, int mode
			template <class T>
			void SceneLoaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_sceneDetection->Loaded<T>(func, ptr);
			}
			// *�V�[���j�����Ăяo���o�^
			// *�߂�l:void
			// *������:CScene* scene
			template <class T>
			void SceneUnloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_sceneDetection->Unloaded<T>(func, ptr);
			}

			auto GetDetection() { return m_sceneDetection.get(); }
			

#ifdef BUILD_MODE

			void ImguiDebug();

#endif // BUILED_MODE
		};
	}
}
using namespace MySpace::SceneManager;


#endif // !__GAME_SCENE_H__