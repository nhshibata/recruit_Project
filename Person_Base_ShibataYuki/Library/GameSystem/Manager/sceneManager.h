//=========================================================
// [sceneManager.h]
//---------------------------------------------------------
// �쐬:2022/06/17
//---------------------------------------------------------
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//--- �C���N���[�h��
#include <winerror.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/instantResourceManager.h>
#include <GameSystem/Scene/sceneTransitionDetection.h>

#pragma region ForwardDeclaration
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
#pragma endregion


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
		class CSceneManager
		{
		private:
			//--- �G�C���A�X
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- �����o�ϐ�	
			std::weak_ptr<CScene> m_pCurrentScene;			// ����Scene
			SceneList m_aScenes;							// Scene�ێ����X�g
			std::string m_currentPath;						// Scene���݃p�X

			CSceneTransitionDetection* m_sceneDetection;	// �V�[���J�ڎ��R�[���o�b�N�N���X
			CCollisionSystem* m_pCollisionSystem;			// �����蔻��Ǘ�
			CDrawSystem* m_pDrawSystem;						// �`��Ǘ�
			CNavMeshBake* m_pNavMesh;						// �i�r���b�V���N���X
			bool m_bTransition;								// �V�[���J�ڃt���O

		private:
			//--- ���ފ֐�
			CSceneManager();
			~CSceneManager();
			
			// *@�V�[���j�����ɌĂяo�����߂̊֐�
			// *@m_bTransition��true
			// *@�ċA�Ăяo�������
			void* FlagOn(CScene* scene)
			{
				m_bTransition = true;
				CSceneManager::Get()->SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
					//std::bind(&CSceneManager::FlagOn, this, std::placeholders::_1));
				return scene ? nullptr : nullptr;
			}

			// *@���X�g�ɒǉ�
			// *@�i�[�ł����false,���ɑ��݂����true
			bool AddList(std::shared_ptr<CScene> add);

		public:
			
			static CSceneManager* Get();
			
			HRESULT Init();
			void Uninit();
			void UpdateScene();
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

			//--- �V�[���ǂݍ��݊֘A
			
			// *@�Z�[�u
			void SaveScene(const std::string filename = "none");

			// *@���[�h
			bool LoadScene(const std::string path);

			// *@�X�^�[�g
			void SetStartScene(std::shared_ptr<CScene> scene)
			{
				m_pCurrentScene = scene;
				m_aScenes.push_back(scene);
			}

			// *@�ʏ�̃V�[���ؑ�
			std::weak_ptr<CScene> SceneTransition(std::string name);
			
			// *@���C���V�[���ؑ�
			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *@�V�[������
			std::shared_ptr<CScene> NewScene(std::string name = std::string());
			
			// *@�V�[������
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}

			// *@�V�[���j��
			// *@�������w��Őؑ�
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		

			//--- �V�[���J�ڎ��o�^�֐�
			
			// *@�V�[���J�ڎ��Ăяo���o�^
			// *@�߂�l:void*
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

			// *@�����񂪈�v����V�[���擾
			std::shared_ptr<CScene> GetSceneByName(std::string name);

			// *@���C���V�[���擾(�����������SP�擾)
			CScene* GetActiveScene()const								{ return m_pCurrentScene.lock().get(); }

			// *@���C���V�[���擾(�������Ȃ��Ő��߲���擾)
			std::shared_ptr<CScene> GetActiveScene(int)const			{ return m_pCurrentScene.lock(); }

			// *@���C���V�[�����O�擾
			std::string GetActiveSceneName()const						{ return m_pCurrentScene.lock()->GetSceneName(); }

			// *@�S�V�[���擾
			std::vector<std::shared_ptr<CScene>> GetAllScene()const		{ return m_aScenes; }

			// *@�ؑ֌Ăяo���׽�̎擾
			inline CSceneTransitionDetection* GetDetection()const		{ return m_sceneDetection; }
			
			// *@�R���W�����V�X�e���̎擾
			inline CCollisionSystem* GetCollisionSystem()const			{ return m_pCollisionSystem; }
			
			// *@�`��V�X�e���̎擾
			inline CDrawSystem* GetDrawSystem()const					{ return m_pDrawSystem; }

			// *@����NavMesh�̎擾
			inline CNavMeshBake* GetNavMesh()const						{ return m_pNavMesh; };

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