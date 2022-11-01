//=========================================================
// [mouse.h]
// 作成:2022/06/26
//=========================================================

//--- インクルード部
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <CoreSystem/Math/MyMath.h>

namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Input
	{
		class Mouse
		{
		public:
			enum EButton
			{
				Left,
				Middle,		// ミドル
				Right,
				MAX
			};

		private:
			static Vector2 m_vCursorPos;
			static Vector2 m_vPrevCursorPos;
			static Vector2 m_vClientPos;
			static Vector2 m_vVel;
			static float m_fWheel;
			static bool m_bWheel;

			static bool curState[(int)EButton::MAX];
			static bool mouseState[(int)EButton::MAX];
			static bool mouseStateTrigger[(int)EButton::MAX];
			static bool mouseStateRelease[(int)EButton::MAX];

		public:

			static void Initialize();
			static void Update();
			static void SetButtonDown(EButton button);
			static void SetButtonUp(EButton button);
			static void SetWheel(float value);
			static void GetPosition(Vector2 * pos);
			static Vector2 & GetPosition();
			static bool IsPress(EButton button);
			static bool IsPressAny();
			static bool IsTriggerAny();
			static bool IsTrigger(EButton button);
			static bool IsRelease(EButton button);
			static void GetMove(Vector2 * move);
			static Vector2 & GetMove();
			static float GetHwheel();

		};
	}
}

#endif // !__MOUSE_H__
