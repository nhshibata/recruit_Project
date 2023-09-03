//==========================================================
// [pauseMenu.h]
//----------------------------------------------------------
// ì¬:2023/06/08
// 
//==========================================================

#ifndef __DARK_PAUSE_MENU_H__
#define __DARK_PAUSE_MENU_H__

#include <GameSystem/Component/Renderer/polygonRenderer.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CPauseMenu : public CComponent
	{
	private:
		// cpppñÌ
		enum class EMenuMode
		{
			None,
			Select,
			Fixed,
		};

	public:
		// àOIðpñÌ
		enum class ESelect
		{
			Title,
			Quit,
			Max
		};

	public:
		static constexpr LPCSTR TAG = "PauseMenu";

	private:
		std::vector<std::weak_ptr<CPolygonRenderer>> m_aPolygon;
		int m_nCursor;
		EMenuMode m_eMode;

	public:
		CPauseMenu();
		CPauseMenu(CGameObject::Ptr ptr);
		~CPauseMenu();

		void Awake();
		void Init();
		void Update();

		// *@Iðµ½©
		int Select();


		void UIEnable(const bool& enable);

#if BUILD_MODE
		void ImGuiDebug();
#endif // BUILD_MODE


	};

}

#endif // !__DARK_PAUSE_MENU_H__