//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �V�[���׽ : ���N���X
//=========================================================
#ifndef __SCENE_H__
#define __SCENE_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/DrawManager.h>
#include <CoreSystem/Util/stl.h>

//--- �O���Q��
namespace MySpace
{
	namespace Game
	{
		/*class CGameObjectManager;
		class CDrawManager;*/
		using MySpace::Game::CGameObjectManager;
		using MySpace::Game::CDrawManager;
	}
	namespace SceneManager
	{
		class CSceneManager;
	}
}

namespace MySpace
{	
	using MySpace::SceneManager::CSceneManager;

	namespace SceneManager
	{			
		using namespace MySpace::Game;

		//--- �N���X��`
		class CScene
		{
			friend class MySpace::SceneManager::CSceneManager;
		protected:
			//--- �����o�ϐ�
			std::shared_ptr<CGameObjectManager> m_objeManager;
			std::shared_ptr<CDrawManager> m_drawManager;

			std::string m_SceneName;
			std::weak_ptr<CScene> m_spPtr;
		private:
			//--- ���ފ֐�
			void SetScene(std::weak_ptr<CScene> scene) 
			{
				m_spPtr = scene; 
				m_objeManager->SetAffiliationScene(m_spPtr.lock());
			}
		public:
			CScene();
			CScene(std::string name);
			virtual ~CScene();
			
			virtual void Init();
			virtual void Uninit();
			virtual void Update();
			virtual void FixedUpdate();
			virtual void Draw();

			void CreateEmptyScene();

			//--- �Z�b�^�[�E�Q�b�^�[
			inline const std::string& GetSceneName() { return m_SceneName; }
			inline void SetSceneName(const std::string name) { m_SceneName = name; }
			inline CGameObjectManager* GetObjManager() { return m_objeManager.get(); }
			inline CDrawManager* GetDrawManager() { return m_drawManager.get(); }
			inline void SetObjManager(std::shared_ptr<CGameObjectManager> mgr) { m_objeManager.reset(); m_objeManager = mgr; }
		};
	}
}
#endif // !__SCENE_H__

