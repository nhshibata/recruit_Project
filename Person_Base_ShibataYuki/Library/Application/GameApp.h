//=========================================================
//作成:2022/05/24
// ゲーム実行ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __GAME_APP_H__
#define __GAME_APP_H__

//--- インクルード部
#include <CoreSystem/Time/fps.h>

// 前方宣言
class Application;

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		class CGameApp
		{
		private:
			//--- メンバ関数
			void BeginRender();
			void EndRender();
			
		public:
			CGameApp();
			~CGameApp();

			void Init();
			void Uninit()const;
			void FixedUpdate()const;
			void InputUpdate();
			void Update();
			void Draw();
		};
	}
}

#endif //!__GAME_APP__