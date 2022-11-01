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
}

namespace MySpace
{	
	namespace SceneManager
	{			
		using namespace MySpace::Game;

		//--- クラス定義
		class CScene
		{
		protected:
			//--- メンバ変数
			std::shared_ptr<CGameObjectManager> m_objeManager;
			std::shared_ptr<CDrawManager> m_drawManager;

			std::string m_SceneName;
		public:
			//--- ﾒﾝﾊﾞ関数
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
			const std::string& GetSceneName() { return m_SceneName; }
			void SetSceneName(const std::string name) { m_SceneName = name; }
			CGameObjectManager* GetObjManager() { return m_objeManager.get(); }
			CDrawManager* GetDrawManager() { return m_drawManager.get(); }
			void SetObjManager(std::shared_ptr<CGameObjectManager> mgr) { m_objeManager.reset(); m_objeManager = mgr; }
		};
	}
}
#endif // !__SCENE_H__

