//==========================================================
// [input.h]
// 2022/06/11 作成
//==========================================================

//--- インクルードガード
#ifndef __GAME_PAD_H__
#define __GAME_PAD_H__

#pragma warning (disable : 4995)
#define NOMINMAX

//--- インクルード部
#include <Windows.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")

namespace MySpace
{
	namespace Input
	{
		//--- クラス定義
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
			//--- メンバ変数
			static inline XINPUT_STATE m_XInputState;				//XBOXコントローラーの状態を格納する
			static inline XINPUT_VIBRATION m_Vibration;				//XBOXコントローラーのバイブレーション
			static inline bool g_bEnable;

			static inline BYTE m_aPadState[(int)EButton::MAX];		// コントローラーの入力情報
			static inline BYTE m_aPadTrigger[(int)EButton::MAX];	// コントローラーのトリガー
			static inline BYTE m_aPadRelease[(int)EButton::MAX];	// コントローラーのリリース

		public:
			//--- メンバ関数
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
