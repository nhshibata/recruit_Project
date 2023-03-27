//=========================================================
// [imGuiContextMenu.h] 
// 作成:2023/03/13
//---------------------------------------------------------
// コンテキストメニュー用
//=========================================================

//--- インクルードガード
#ifndef __IMGUI_CONTEXT_MENU_H__
#define __IMGUI_CONTEXT_MENU_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Math/myVector.h>

#if BUILD_MODE

#pragma region forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}
	namespace Debug
	{
		class ImGuiManager;
	}
}
#pragma endregion


namespace MySpace
{
	namespace Debug
	{
		void PopUpGameObjectMenu(MySpace::Game::CGameObject* pObj);

		std::shared_ptr<MySpace::Game::CGameObject> CopyGameObject();
	}
}

#endif //BUILD_MODE

#endif // !__IMGUI_CONTEXT_MENU_H__