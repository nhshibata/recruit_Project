//=========================================================
// [imGuiContextMenu.h] 
// �쐬:2023/03/13
//---------------------------------------------------------
// �R���e�L�X�g���j���[�p
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __IMGUI_CONTEXT_MENU_H__
#define __IMGUI_CONTEXT_MENU_H__

//--- �C���N���[�h��
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