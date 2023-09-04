//==========================================================
// [floor.h]
//----------------------------------------------------------
// �쐬:2023/05/19 
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
		//--- �����o�֐�
		CFloor();
		CFloor(CGameObject::Ptr ptr);

		void Awake();
		void Update();
	};

}

#endif // !__FLOOR_COMPONENT_H__
