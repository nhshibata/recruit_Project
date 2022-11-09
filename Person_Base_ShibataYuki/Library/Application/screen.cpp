//=========================================================
// [screen.h]
// �쐬:2022/07/03
//=========================================================

#include <Application/screen.h>

//--- �萔��`
#define SCREEN_WIDTH		1280.0f
#define SCREEN_HIGHT		720.0f	//960

namespace MySpace
{
	namespace System
	{
		using namespace MySpace::MyMath;

		//--- �ÓI�����o�ϐ�
		// �����ݒ�
		float CScreen::m_fWidth = SCREEN_WIDTH;
		float CScreen::m_fHeight = SCREEN_HIGHT;
		Matrix4x4 CScreen::m_mtxScreen = Matrix4x4(
			SCREEN_WIDTH / 2, 0.0f, 0.0f, 0.0f,
			0.0f, -SCREEN_HIGHT / 2, 0.0f, 0.0f,
			SCREEN_WIDTH / 2, 0.0f, 1.0f, 0.0f,
			SCREEN_WIDTH / 2, SCREEN_HIGHT / 2, 0.0f, 1.0f
		);
	}
}