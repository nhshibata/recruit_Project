//=========================================================
//作成:2022/04/19 (火曜日)
// シーンｸﾗｽ : 基底クラス
//=========================================================
#ifndef __SCENE_H__
#define __SCENE_H__

//--- インクルード部
#include <GameSystem/Manager/gameObjectManager.h>
#include <CoreSystem/Util/stl.h>

//--- 前方宣言
namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CGameObjectManager;
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
			std::shared_ptr<CGameObjectManager> m_pObjeManager;		// GameObject所持

			std::string m_SceneName;
			std::weak_ptr<CScene> m_spPtr;

		public:
			//--- ﾒﾝﾊﾞ関数
			CScene();
			CScene(std::string name);
			virtual ~CScene();
			
			virtual void Init(std::weak_ptr<CScene> scene);
			virtual void Uninit();
			virtual void Update();
			virtual void FixedUpdate();
			virtual void Draw();

			void CreateEmptyScene();

			//--- セッター・ゲッター
			inline const std::string& GetSceneName() { return m_SceneName; }
			inline CGameObjectManager* GetObjManager() { return m_pObjeManager.get(); }

			inline void SetSceneName(const std::string name) { m_SceneName = name; }
			inline void SetObjManager(std::shared_ptr<CGameObjectManager> mgr) { m_pObjeManager.reset(); m_pObjeManager = mgr; }
		};
	}
}
#endif // !__SCENE_H__

