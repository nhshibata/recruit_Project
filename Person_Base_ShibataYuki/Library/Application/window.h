//=========================================================
// [window.h] 
// �쐬:2022/05/24
//---------------------------------------------------------
// �E�B���h�E�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __WINDOW_H__
#define __WINDOW_H__

#define NOMINMAX

//--- �C���N���[�h��
#include <Windows.h>

#include <CoreSystem/Math/myVector.h>
#include <memory>

namespace MySpace
{
	namespace System
	{
		using namespace MySpace::MyMath;

		//--- �N���X��`
		class CWindow
		{
		private:
			//--- �����o�ϐ�
			float m_fScreenWidth;
			float m_fScreenHeight;
			float m_fClientWidth;
			float m_fClientHeight;

			HWND m_hwnd;
			MSG m_Message;
		
		public:
			//--- �����o�֐�
			CWindow() 
			{
				m_fScreenWidth = m_fScreenHeight = m_fClientWidth = m_fClientHeight = 0.0f;
			};
			~CWindow() = default;

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

			// �X�N���[��
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