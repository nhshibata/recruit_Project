//========================================================
// [actionPlayer.h]
//--------------------------------------------------------
// 作成:2023/03/17
//--------------------------------------------------------
//
//========================================================

#ifndef __ACTION_PLAYER_H__
#define __ACTION_PLAYER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		class CRigidbody;
	}
}

namespace MySpace
{
	namespace Game
	{

		class CActionPlayer : public CComponent
		{
#pragma region Serealize
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("actionPlayer", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_fSpeed), CEREAL_NVP(m_fJump), CEREAL_NVP(m_bLand)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("actionPlayer", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_fSpeed), CEREAL_NVP(m_fJump), CEREAL_NVP(m_bLand)
				);
			}
#pragma endregion
		private:
			float m_fSpeed;
			float m_fJump;
			bool m_bLand;
			CRigidbody* m_rb;

		public:
			CActionPlayer();
			CActionPlayer(CGameObject::Ptr);
			~CActionPlayer();

			void OnLoad();
			void Awake();
			void Init();
			void Update();

			void OnCollisionEnter(CGameObject* obj);
			void OnCollisionStay(CGameObject* obj);
			void OnCollisionExit(CGameObject* obj);

#if BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE


		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CActionPlayer);

#endif // !__SPELL_PLAYER_H__

