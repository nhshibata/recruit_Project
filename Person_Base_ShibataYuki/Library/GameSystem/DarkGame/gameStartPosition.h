//==========================================================
// [gameStartPosition.h]
//----------------------------------------------------------
// çÏê¨:2023/05/21
// 
//==========================================================

#ifndef __GAMESTART_POSITION_H__
#define __GAMESTART_POSITION_H__

#include <GameSystem/Component/component.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CGameStartPosition : public CComponent
	{
	public:
		static constexpr LPCSTR TAG = "StartPos";

	private:
		Vector3 m_vPosition;

	public:
		//--- ÉÅÉìÉoä÷êî
		CGameStartPosition();
		CGameStartPosition(CGameObject::Ptr ptr);

		void Awake();
		void Init();
		void Update();

		Vector3 GetPos()const { return m_vPosition; }
	};

}

#endif // !__GAMESTART_POSITION_H__