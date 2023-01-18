//========================================================
// [gameSceneManager.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__

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

	class CGameSceneManager : public CComponent
	{
	private:
		CSpellPlayer* m_pPlayer;
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

#endif // !__GAME_SCENE_MANAGER_H__