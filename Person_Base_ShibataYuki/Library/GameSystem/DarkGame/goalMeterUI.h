//==========================================================
// [goalMeterUI.h]
//----------------------------------------------------------
// çÏê¨:2023/05/21
// 
//==========================================================

#ifndef __GAME_METER_UI_H__
#define __GAME_METER_UI_H__

#include <GameSystem/DarkGame/darkPlayer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CGameMeterUI : public CComponent
	{
	private:
		std::weak_ptr<CDarkPlayer> m_pDarkPlayer;
		std::weak_ptr<CTextRenderer> m_pText;
		std::weak_ptr<CTransform> m_pGoalTransform;

	public:
		//--- ÉÅÉìÉoä÷êî
		CGameMeterUI();
		CGameMeterUI(CGameObject::Ptr ptr);

		void Awake();
		void Init();
		void Update();
	};

}

#endif // !__GAME_METER_UI_H__