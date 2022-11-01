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
}

namespace MySpace
{	
	namespace SceneManager
	{			
		using namespace MySpace::Game;

		//--- �N���X��`
		class CScene
		{
		protected:
			//--- �����o�ϐ�
			std::shared_ptr<CGameObjectManager> m_objeManager;
			std::shared_ptr<CDrawManager> m_drawManager;

			std::string m_SceneName;
		public:
			//--- ���ފ֐�
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
			const std::string& GetSceneName() { return m_SceneName; }
			void SetSceneName(const std::string name) { m_SceneName = name; }
			CGameObjectManager* GetObjManager() { return m_objeManager.get(); }
			CDrawManager* GetDrawManager() { return m_drawManager.get(); }
			void SetObjManager(std::shared_ptr<CGameObjectManager> mgr) { m_objeManager.reset(); m_objeManager = mgr; }
		};
	}
}
#endif // !__SCENE_H__

