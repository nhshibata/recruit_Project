//========================================================
// [spellPlayer.h]
//--------------------------------------------------------
// 作成:2023/01/02
//--------------------------------------------------------
//
//========================================================

#ifndef __SPELL_PLAYER_H__
#define __SPELL_PLAYER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

//--- 前方宣言
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}
}

namespace Spell
{
	using namespace MySpace::Game;

	class CSpellPlayer : public CComponent
	{
#pragma region Serealize
		//--- シリアライズ
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("SpellPlayer", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_fSpeed), CEREAL_NVP(m_fJump), CEREAL_NVP(m_bLand)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("SpellPlayer", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_fSpeed), CEREAL_NVP(m_fJump), CEREAL_NVP(m_bLand)
			);
		}
#pragma endregion
	private:
		float m_fSpeed;
		float m_fJump;
		bool m_bLand;

	public:
		CSpellPlayer();
		CSpellPlayer(CGameObject::Ptr);
		~CSpellPlayer();

		void Awake();
		void Init();
		void Update();

		void OnCollisionEnter(CGameObject * obj);
		void OnCollisionStay(CGameObject * obj);
		
	};

}

CEREAL_REGISTER_TYPE(Spell::CSpellPlayer);

#endif // !__SPELL_PLAYER_H__

