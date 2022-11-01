//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 派生クラス
//=========================================================
//--- インクルードガード
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

//--- インクルード部
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- クラス定義
		class CTitleScene : public CScene
		{
		private:

		public:
			CTitleScene();
			~CTitleScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();
		};
	}
}

#endif // !__TITLE_SCENE_H__

