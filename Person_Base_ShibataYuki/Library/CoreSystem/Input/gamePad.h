//==========================================================
// [input.h]
// 2022/06/11 �쐬
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_PAD_H__
#define __GAME_PAD_H__

#pragma warning (disable : 4995)
#define NOMINMAX

//--- �C���N���[�h��
#include <Windows.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")

namespace MySpace
{
	namespace Input
	{
		//--- �N���X��`
		class CGamePad
		{
		public:
			enum class EButton
			{
				DPAD_UP,
				DPAD_DOWN,
				DPAD_LEFT,
				DPAD_RIGHT,
				START,
				BACK,
				LEFT_SHOULDER,
				RIGHT_SHOULDER,
				A,
				B,
				X,
				Y,
				THUMB_L_UP,
				THUMB_L_DOWN,
				THUMB_L_LEFT,
				THUMB_L_RIGHT,
				THUMB_R_UP,
				THUMB_R_DOWN,
				THUMB_R_LEFT,
				THUMB_R_RIGHT,
				LEFT_TRIGGER,
				RIGHT_TRIGGER,
				MAX
			};
		private:
			//--- �����o�ϐ�
			static inline XINPUT_STATE m_XInputState;				//XBOX�R���g���[���[�̏�Ԃ��i�[����
			static inline XINPUT_VIBRATION m_Vibration;				//XBOX�R���g���[���[�̃o�C�u���[�V����
			static inline bool g_bEnable;

			static inline BYTE m_aPadState[(int)EButton::MAX];		// �R���g���[���[�̓��͏��
			static inline BYTE m_aPadTrigger[(int)EButton::MAX];	// �R���g���[���[�̃g���K�[
			static inline BYTE m_aPadRelease[(int)EButton::MAX];	// �R���g���[���[�̃����[�X

		public:
			//--- �����o�֐�
			CGamePad();
			~CGamePad();

			static void Init();
			static void Uninit();
			static void Update();

			static bool Enable();

			static bool IsPress(EButton nKey);
			static bool IsTrigger(EButton nKey);
			static bool IsRelease(EButton nKey);

			static float ThumbLX();
			static float ThumbLY();
			static float ThumbRX();
			static float ThumbRY();
		};
	}
}
#endif // __INPUT_H__
