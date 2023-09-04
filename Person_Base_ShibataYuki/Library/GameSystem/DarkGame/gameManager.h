//==========================================================
// [gameManager.h]
//----------------------------------------------------------
// 作成:2023/05/21
// 
//==========================================================

#ifndef __DARK_GAME_MANAGER_H__
#define __DARK_GAME_MANAGER_H__

#include <GameSystem/Component/component.h>
#include <GameSystem/DarkGame/imageSwitch.h>

namespace DarkGame
{
	class CDarkPlayer;
	class CGameGoal;
	class CGameStartPosition;
	class CPauseMenu;
}

namespace DarkGame
{
	using namespace MySpace::Game;

	class CGameManager : public CComponent
	{
	public:
		enum class EState
		{
			Start = 0,
			Play,
			Over,
			Goal,
			Pause,
			Quit, // 使う?
		};

	public:
		static constexpr LPCSTR TAG = "GameManager";

	private:
		EState m_eCurrentState;
		std::weak_ptr<CDarkPlayer> m_pPlayer;
		std::weak_ptr<CGameGoal> m_pGoal;
		std::weak_ptr<CImageSwitch> m_pUI;
		std::weak_ptr<CPauseMenu> m_pPause;
		std::weak_ptr<CGameStartPosition> m_pStartPos; // FIXME:クラス化する必要性

	public:
		//--- メンバ関数
		CGameManager();
		CGameManager(CGameObject::Ptr ptr);

		void OnLoad() { Awake(); };
		void Awake();
		void Init();
		void Update();

		// *@動くオブジェクトの停止
		void StopMode(const bool& stop);

		EState GetState()const { return m_eCurrentState; }
		void SetState(const EState);

		// *@プレイ中か
		bool IsPlay()const { return m_eCurrentState == EState::Play; }
	};

}

#endif // !__DARK_GAME_MANAGER_H__
