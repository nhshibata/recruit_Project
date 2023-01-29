//========================================================
// [spellPlayer.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __SPELL_PLAYER_H__
#define __SPELL_PLAYER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

namespace Spell
{
	using namespace MySpace::Game;


	class CSpellPlayer : public CComponent
	{
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

#endif // !__SPELL_PLAYER_H__

