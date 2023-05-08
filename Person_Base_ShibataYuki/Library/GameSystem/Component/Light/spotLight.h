//=========================================================
// [spotLight.h]
// 作成:2023/04/13
// 
//=========================================================

//--- インクルードガード
#ifndef __SPOT_LIGHT_COMPONENT_H__
#define __SPOT_LIGHT_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Light/pointLight.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CSpotLight : public CLight
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("SpotLight", cereal::base_class<CLight>(this)),
						CEREAL_NVP(m_fRange), CEREAL_NVP(m_vColor)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("SpotLight", cereal::base_class<CLight>(this)),
						CEREAL_NVP(m_fRange), CEREAL_NVP(m_vColor)
				);
			}
		private:
			Color m_vColor;
			float m_fRange;

		public:
			//--- ﾒﾝﾊﾞ関数
			CSpotLight();
			CSpotLight(std::shared_ptr<CGameObject> owner);
			~CSpotLight();

			void OnLoad();
			virtual void Awake();			// 初期化 他コンポーネントの取得などを行う
			//void LateUpdate();			// 遅い更新

			inline Color GetColor()const { return m_vColor; }
			inline float GetRange()const { return m_fRange; }
			
			inline void SetColor(Color value) { m_vColor = value; }
			inline void SetRange(float value) { m_fRange = value; }


#ifdef BUILD_MODE
			// セッター・ゲッター
			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSpotLight)

#endif // !__PLAYER_H__
