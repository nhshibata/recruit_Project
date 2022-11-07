//=========================================================
//作成:2022/04/19 (火曜日)
// シーンｸﾗｽ : 基底クラス
//=========================================================
#ifndef __SCENE_H__
#define __SCENE_H__

//--- インクルード部
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/DrawManager.h>
#include <CoreSystem/Util/stl.h>

//--- 前方参照
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

		//--- クラス定義
		class CScene
		{
			friend class MySpace::SceneManager::CSceneManager;
		protected:
			//--- メンバ変数
			std::shared_ptr<CGameObjectManager> m_objeManager;
			std::shared_ptr<CDrawManager> m_drawManager;

			std::string m_SceneName;
			std::weak_ptr<CScene> m_spPtr;
		private:
			//--- ﾒﾝﾊﾞ関数
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

			//--- セッター・ゲッター
			inline const std::string& GetSceneName() { return m_SceneName; }
			inline void SetSceneName(const std::string name) { m_SceneName = name; }
			inline CGameObjectManager* GetObjManager() { return m_objeManager.get(); }
			inline CDrawManager* GetDrawManager() { return m_drawManager.get(); }
			inline void SetObjManager(std::shared_ptr<CGameObjectManager> mgr) { m_objeManager.reset(); m_objeManager = mgr; }
		};
	}
}
#endif // !__SCENE_H__

