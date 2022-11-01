//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 派生クラス
//=========================================================
//--- インクルードガード
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

//--- インクルード部
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- クラス定義
		class CGameScene : public CScene
		{
		private:

		protected:
			//--- ﾒﾝﾊﾞ関数

		public:
			CGameScene();
			~CGameScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();

		};
	}
}
#endif // !__GAME_SCENE_H__