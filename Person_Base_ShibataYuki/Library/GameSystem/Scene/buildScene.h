//=========================================================
// シーンｸﾗｽ : 派生クラス
//=========================================================
//--- インクルードガード
#ifndef __BUILD_SCENE_H__
#define __BUILD_SCENE_H__

//--- インクルード部
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- クラス定義
		class CBuildScene : public CScene
		{
		private:

		public:
			CBuildScene();
			CBuildScene(std::string name);
			~CBuildScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();
		};
	}
}

#endif // !__BUILD_SCENE_H__

