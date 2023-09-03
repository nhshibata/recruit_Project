//==========================================================
// [gameGoal.h]
//----------------------------------------------------------
// 作成:2023/05/19 
//----------------------------------------------------------
// ゲーム終了条件を満たすもの
//==========================================================

#ifndef __GAME_GOAL_COMPONENT_H__
#define __GAME_GOAL_COMPONENT_H__

#include <GameSystem/Component/component.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CGameGoal : public CComponent
	{
	public:
		static constexpr LPCSTR TAG = "Goal";
	private:
		bool m_bGoal;

	public:
		//--- メンバ関数
		CGameGoal();
		CGameGoal(CGameObject::Ptr ptr);

		void Awake();
		void Update();

		void OnTriggerEnter(CGameObject* obj);

		const bool IsGoal()const { return m_bGoal; }
	};

}

#endif // !__GAME_GOAL_COMPONENT_H__
