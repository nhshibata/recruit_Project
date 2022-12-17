//=========================================================
// [mouse.h]
// �쐬:2022/06/26
//=========================================================

//--- �C���N���[�h��
#ifndef __MOUSE_H__
#define __MOUSE_H__

//--- �C���N���[�h��
#include <CoreSystem/Math/MyMath.h>

namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Input
	{
		class Mouse
		{
		public:
			//--- �񋓑̒�`
			enum EButton
			{
				Left,
				Middle,		// �~�h��
				Right,
				MAX
			};

		private:
			//--- �����o�ϐ�
			static inline Vector2 m_vCursorPos;
			static inline Vector2 m_vPrevCursorPos;
			static inline Vector2 m_vClientPos;
			static inline Vector2 m_vVel;
			static inline float m_fWheel;
			static inline bool m_bWheel;
			
			static inline bool curState[(int)EButton::MAX];
			static inline bool mouseState[(int)EButton::MAX];
			static inline bool mouseStateTrigger[(int)EButton::MAX];
			static inline bool mouseStateRelease[(int)EButton::MAX];

		public:
			//--- �����o�֐�
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
