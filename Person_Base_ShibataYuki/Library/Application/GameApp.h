//=========================================================
// [GameApp.h]
//---------------------------------------------------------
//作成:2022/05/24
//---------------------------------------------------------
// ゲーム実行ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __GAME_APP_H__
#define __GAME_APP_H__

//--- インクルード部
#include <CoreSystem/Time/fps.h>

//--- 前方宣言
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
			void BeginRender(Application*);
			void EndRender(Application*);
			
		public:
			CGameApp();
			~CGameApp();

			HRESULT Init(Application*);
			void Uninit(Application*)const;
			void FixedUpdate(Application*)const;
			void InputUpdate();
			void Run(Application*);
			void Draw(Application*);
		};
	}
}

#endif //!__GAME_APP__