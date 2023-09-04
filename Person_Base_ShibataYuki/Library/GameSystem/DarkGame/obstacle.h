//==========================================================
// [obstacle.h]
//----------------------------------------------------------
// çÏê¨:2023/05/19 
// 
//==========================================================

#ifndef __OBSTACLE_COMPONENT_H__
#define __OBSTACLE_COMPONENT_H__

#include <GameSystem/Component/component.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CObstacle : public CComponent
	{
	public:
		static constexpr LPCSTR TAG = "Obstacle";

	public:
		//--- ÉÅÉìÉoä÷êî
		CObstacle();
		CObstacle(CGameObject::Ptr ptr);

		void Awake();
		void Update();
	};

}

#endif // !__OBSTACLE_COMPONENT_H__