//========================================================
// [gameManager.h]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

//--- �C���N���[�h��
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

	class CGameManager : public CComponent
	{
	private:
		CGameSceneManager* m_pGameScene;
		std::weak_ptr<CFadeController> m_pFade;

	public:
		CGameManager();
		CGameManager(CGameObject::Ptr owner);
		~CGameManager();

		void Awake();
		void Update();

		// *@�V�[���ؑ֎��Ăяo���p
		void* SceneResponce(MySpace::SceneManager::CScene* prev, MySpace::SceneManager::CScene* next);
	};

}

#endif // !__GAME_MANAGER_H__