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
			void BeginRender(Application& app);
			void EndRender(Application& app);
			
		public:
			CGameApp();
			~CGameApp();

			void Init(Application& app);
			void Uninit(Application& app)const;
			void FixedUpdate(Application& app)const;
			void InputUpdate();
			void Update(Application& app)const;
			void Draw(Application& app);
		};
	}
}

#endif //!__GAME_APP__