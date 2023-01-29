//========================================================
// [gameSceneManager.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __SPELL_GAME_SCENE_MANAGER_H__
#define __SPELL_GAME_SCENE_MANAGER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

#pragma region ForwardDeclaration

class CFadeController;

namespace Spell
{
	class CSpellPlayer;
}

#pragma endregion


namespace Spell
{
	using namespace MySpace::Game;

	class CGameSceneManager : public CComponent
	{
	private:
		std::weak_ptr<CSpellPlayer> m_pPlayer;
		std::weak_ptr<CFadeController> m_pFade;

	public:
		CGameSceneManager();
		CGameSceneManager(CGameObject::Ptr owner);
		~CGameSceneManager();

		void Awake();
		void Init();
		void Update();

		void TimeFunc();
	};

}

#endif // !__SPELL_GAME_SCENE_MANAGER_H__