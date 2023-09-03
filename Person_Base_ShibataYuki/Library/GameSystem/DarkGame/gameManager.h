//==========================================================
// [gameManager.h]
//----------------------------------------------------------
// �쐬:2023/05/21
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
			Quit, // �g��?
		};

	public:
		static constexpr LPCSTR TAG = "GameManager";

	private:
		EState m_eCurrentState;
		std::weak_ptr<CDarkPlayer> m_pPlayer;
		std::weak_ptr<CGameGoal> m_pGoal;
		std::weak_ptr<CImageSwitch> m_pUI;
		std::weak_ptr<CPauseMenu> m_pPause;
		std::weak_ptr<CGameStartPosition> m_pStartPos; // FIXME:�N���X������K�v��

	public:
		//--- �����o�֐�
		CGameManager();
		CGameManager(CGameObject::Ptr ptr);

		void OnLoad() { Awake(); };
		void Awake();
		void Init();
		void Update();

		// *@�����I�u�W�F�N�g�̒�~
		void StopMode(const bool& stop);

		EState GetState()const { return m_eCurrentState; }
		void SetState(const EState);

		// *@�v���C����
		bool IsPlay()const { return m_eCurrentState == EState::Play; }
	};

}

#endif // !__DARK_GAME_MANAGER_H__
