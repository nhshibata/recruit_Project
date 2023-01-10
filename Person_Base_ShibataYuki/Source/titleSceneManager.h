//========================================================
// [titleSceneManager.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __TITLE_SCENE_MANAGER_H__
#define __TITLE_SCENE_MANAGER_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

#pragma region ForwardDeclaration

class CFadeController;
namespace MySpace
{
	namespace Game
	{
		class CPolygonRenderer;
	}
}

#pragma endregion


namespace Spell
{

	class CTitleSceneManager : public CComponent
	{
	private:
		std::weak_ptr<CFadeController> m_pFade;
		std::weak_ptr<CPolygonRenderer> m_pButton;

	public:
		CTitleSceneManager();
		CTitleSceneManager(CGameObject::Ptr owner);
		~CTitleSceneManager();

		void Awake();
		void Init();
		void Update();

	};

}

#endif // !__TITLE_SCENE_MANAGER_H__