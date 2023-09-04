//==========================================================
// [lightCollisionCondition.h]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認用
// 条件を確認し、判定を保持する
//==========================================================

#ifndef __LIGHT_COLLISION_CONDITION_H__
#define __LIGHT_COLLISION_CONDITION_H__

#include <GameSystem/DarkGame/lightCollision.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollisionCondition : public CLightCollision
	{
	private:
		std::string m_SpecName;
		bool m_bHit;

	public:
		//--- メンバ関数
		CLightCollisionCondition();
		CLightCollisionCondition(CGameObject::Ptr ptr);
		virtual ~CLightCollisionCondition();

		void Awake();
		void Update();

		// *@情報のクリア
		void Refresh();

		// *@条件を満たしているか
		bool IsRange(std::string spec);

		// *@条件を満たした場合はtrue
		bool Check(const CLightCollision&);

		// *@条件名設定
		void SetCondition(const std::string name) { m_SpecName = name; }

	};

}

#endif // !__LIGHT_COLLISION_CONDITION_H__
