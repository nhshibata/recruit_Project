//==========================================================
// [UIObj.h]
// 2022/06/07 作成
//----------------------------------------------------------
// TODO: 作りかけ
//==========================================================

//--- インクルードガード
#ifndef __UI_GAME_OBJECT_H__
#define __UI_GAME_OBJECT_H__

//---インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Math/MyMath.h>
#include <GameSystem/Component/Transform/rectTransform.h>

namespace MySpace
{
	namespace Game
	{
		// <Summary>
		// UI表示用のオブジェクト
		// 2Dのため、RectTransformをもつ
		// </Summary>
		//--- クラス定義
		class CUIGameObject final : public CGameObject
		{
		private:
			RectTransWeakPtr m_RectTransform;
		public:
			CUIGameObject();
			~CUIGameObject();

			virtual void Init();
			virtual void Update();
			virtual void LateUpdate();			// ゲームオブジェクトごとの更新
			virtual void FixedUpdate();		// 一定時間の更新
			virtual void Draw();
		};
	}
}
#endif // !__UI_OBJ_H__
