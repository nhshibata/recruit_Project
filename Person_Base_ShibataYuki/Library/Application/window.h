//=========================================================
// [window.h] 
// 作成:2022/05/24
//---------------------------------------------------------
// ウィンドウｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __WINDOW_H__
#define __WINDOW_H__

#define NOMINMAX

//--- インクルード部
#include <Windows.h>

#include <CoreSystem/Math/myVector.h>
#include <CoreSystem/systemBase.h>

#include <memory>

namespace MySpace
{
	namespace System
	{
		using namespace MySpace::MyMath;

		//--- クラス定義
		class CWindow : public CSystemBase
		{
		private:
			//--- メンバ変数
			float m_fScreenWidth;
			float m_fScreenHeight;
			float m_fClientWidth;
			float m_fClientHeight;

			HWND m_hwnd;
			MSG m_Message;
		
		public:
			//--- メンバ関数
			CWindow() 
				:m_fScreenWidth(0),m_fScreenHeight(0), m_fClientWidth(0), m_fClientHeight(0),m_hwnd(0)/*,m_Message()*/
			{
				
			};
			~CWindow() = default;

			static CWindow * Get();

			bool ExecMessage();
			bool RegisterClass(
				HINSTANCE	  hInstance,
				const char*	  h_ClassName,
				unsigned long h_Style);
			void SetWindow(
				HINSTANCE		h_Instance,
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
			const HWND& GetHWND() { return m_hwnd; }
			long GetMessage()const { return m_Message.message; };

			void SetScreenSize(Vector2 size) { m_fScreenWidth = size.x; m_fScreenHeight = size.y; }
			void SetClientSize(Vector2 size) { m_fClientWidth = size.x; m_fClientHeight = size.y; }
		};
	};
};

#endif // !__WINDOW_H__