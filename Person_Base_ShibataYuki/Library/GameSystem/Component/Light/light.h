//=========================================================
// [light.h]
// 作成:2022/06/27
// 
//=========================================================

//--- インクルードガード
#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CLight : public CComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("light", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bEnable)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("light", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bEnable)
				);
			}
		private:
			bool m_bEnable;			// ライティング有効/無効
			int m_nSystemIdx;
			static inline CLight* m_pMainLight = nullptr;

		public:
			//--- ﾒﾝﾊﾞ関数
			CLight();
			CLight(std::shared_ptr<CGameObject> owner);
			~CLight();

			void OnLoad();
			virtual void Awake();			// 初期化 他コンポーネントの取得などを行う
			//void LateUpdate();			// 遅い更新

			inline void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }
			inline void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }
			inline bool IsEnable() { return m_bEnable; }

			static CLight* GetMain();
			static void Set(CLight* pLight = nullptr);

#ifdef BUILD_MODE
			// セッター・ゲッター
			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CLight)

#endif // !__PLAYER_H__
