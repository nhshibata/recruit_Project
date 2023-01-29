//========================================================
// [gameManager.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __SPELL_GAME_MANAGER_H__
#define __SPELL_GAME_MANAGER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

#pragma region ForwardDeclaration

class CFadeController;

namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}

namespace Spell
{
	class CGameSceneManager;
}

#pragma endregion


namespace Spell
{
	using namespace MySpace::Game;

	class CGameManager : public CComponent
	{
	private:
		std::weak_ptr<CGameSceneManager> m_pGameScene;
		std::weak_ptr<CFadeController> m_pFade;

	public:
		CGameManager();
		CGameManager(CGameObject::Ptr owner);
		~CGameManager();

		virtual void Awake()override;
		virtual void Init()override;
		void Update();

		// *@シーン切替時呼び出し用
		void* SceneResponce(MySpace::SceneManager::CScene* prev, MySpace::SceneManager::CScene* next);
	};

}

#endif // !__SPELL_GAME_MANAGER_H__