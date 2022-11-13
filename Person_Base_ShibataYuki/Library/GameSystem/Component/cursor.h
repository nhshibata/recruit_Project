//=========================================================
// 作成:2022/04/21
// カーソル: 部品ｸﾗｽ
//=========================================================
//--- インクルードガード
#ifndef __CURSOR_COMPONENT_H__
#define __CURSOR_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CCursor : public CComponent
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("cursor", cereal::base_class<CComponent>(this)), CEREAL_NVP(m_nPoint),
					CEREAL_NVP(m_nMax),CEREAL_NVP(m_bAuthority)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("cursor", cereal::base_class<CComponent>(this)), CEREAL_NVP(m_nPoint),
					CEREAL_NVP(m_nMax), CEREAL_NVP(m_bAuthority)
				);
			}

		private:
			//--- メンバ変数
			bool m_bInput;				// カーソルを移動しない時用
			int m_nPoint;				// 
			int m_nOldPoint;			// 
			int m_nMax;					// 最大数

			bool m_bAuthority;			// 操作権限

		public:
			//--- ﾒﾝﾊﾞ関数
			CCursor();
			CCursor(std::shared_ptr<CGameObject> owner);
			~CCursor();

			void Init();
			void Update();

			void InputSwitch() { m_bInput ^= true; };

			//--- セッター・ゲッター
			int GetPos() { return m_nPoint; };
			int GetOldPos() { return m_nOldPoint; };

			void SetMax(int value) { m_nMax = value; };
			void SetAuthority(bool flg) { m_bAuthority = flg; }
			bool IsAuthority() { return m_bAuthority; }
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CCursor)

#endif // !__CURSOR_H__

