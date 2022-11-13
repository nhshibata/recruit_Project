//=========================================================
// 作成:2022/06/27
// 
// 
//=========================================================

//--- インクルードガード
#ifndef __DIRECTIONAL_LIGHT_COMPONENT_H__
#define __DIRECTIONAL_LIGHT_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Light/light.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CDirectionalLight : public CLight
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("directionalLight", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_diffuse), CEREAL_NVP(m_ambient), CEREAL_NVP(m_specular), CEREAL_NVP(m_direction)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("directionalLight", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_diffuse), CEREAL_NVP(m_ambient), CEREAL_NVP(m_specular), CEREAL_NVP(m_direction)
				);
			}

		private:
			//--- メンバ変数
			Vector4 m_diffuse;
			Vector4 m_ambient;
			Vector4 m_specular;
			Vector3 m_direction;

		public:
			//--- ﾒﾝﾊﾞ関数
			CDirectionalLight() {};
			CDirectionalLight(std::shared_ptr<CGameObject> owner);
			~CDirectionalLight();

			void Init();				// 初期化 他コンポーネントの取得などを行う

			inline XMFLOAT4& GetDiffuse() { return m_diffuse; }
			inline XMFLOAT4& GetAmbient() { return m_ambient; }
			inline XMFLOAT4& GetSpecular() { return m_specular; }
			XMFLOAT3& GetDir();

#ifdef BUILD_MODE

			// セッター・ゲッター
			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CDirectionalLight)

#endif // !__PLAYER_H__
