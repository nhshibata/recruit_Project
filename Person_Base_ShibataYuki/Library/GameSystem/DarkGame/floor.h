//==========================================================
// [floor.h]
//----------------------------------------------------------
// 作成:2023/05/19 
// 
//==========================================================

#ifndef __FLOOR_COMPONENT_H__
#define __FLOOR_COMPONENT_H__

#include <GameSystem/Component/component.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CFloor : public CComponent
	{
	public:
		static constexpr LPCSTR TAG = "Floor";

	public:
		//--- メンバ関数
		CFloor();
		CFloor(CGameObject::Ptr ptr);

		void Awake();
		void Update();
	};

}

#endif // !__FLOOR_COMPONENT_H__
