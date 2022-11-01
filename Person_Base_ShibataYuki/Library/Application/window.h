//=========================================================
// [window.h] 
// 作成: / /
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __WINDOW_H__
#define __WINDOW_H__


//--- インクルード部
#define NOMINMAX
#include <Windows.h>

#include <CoreSystem/system.h>
#include <CoreSystem/Math/myVector.h>
//#include <Application/main.h>
#include <memory>

namespace MySpace
{
	namespace System
	{
		//--- 前方参照
	}
}

namespace MySpace
{
	namespace System
	{
		using namespace MySpace::MyMath;

		//--- クラス定義
		class CWindow : public CAppSystem<CWindow>
		{
			friend class CAppSystem<CWindow>;
			friend class CSingleton<CWindow>;
		private:
			float m_fScreenWidth;
			float m_fScreenHeight;
			float m_fClientWidth;
			float m_fClientHeight;

			HWND m_hwnd;
			MSG m_Message;

			CWindow() {};
			//~CWindow();
		public:
			void Close(const char* name,HINSTANCE inst);

			bool ExecMessage();
			bool RegisterClass(HINSTANCE	  hInstance,
				const char*	  h_ClassName,
				unsigned long h_Style);
			void SetWindow(HINSTANCE		h_Instance,
				unsigned long	h_Style,
				unsigned long	h_ExStyle,
				HWND			h_Parent,
				long			h_Width,
				long			h_Height,
				const char*		h_ClassName,
				const char*		h_Title,
				bool			FULLSCREEN);

			// スクリーン
			const Vector2 GetScreenSize() { return Vector2(m_fScreenWidth, m_fScreenHeight); }
			const Vector2 GetClientSize() { return Vector2(m_fClientWidth, m_fClientHeight); }
			void SetScreenSize(Vector2 size) { m_fScreenWidth = size.x; m_fScreenHeight = size.y; }
			void SetClientSize(Vector2 size) { m_fClientWidth = size.x; m_fClientHeight = size.y; }

			const HWND& GetHWND() { return m_hwnd; }
			long GetMessage()const { return m_Message.message; };
		};
	};
};

#endif // !__WINDOW_H__