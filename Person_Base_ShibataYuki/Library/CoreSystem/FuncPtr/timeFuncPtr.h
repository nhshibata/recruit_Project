//==========================================================
// [timeFuncPtr.h]
// 作成:2022/06/23
// 時間経過で関数を実行する
//==========================================================

//--- インクルードガード
#ifndef __TIME_FUNC_PTR_H__
#define __TIME_FUNC_PTR_H__

//--- インクルード部
#include <CoreSystem/FuncPtr/funcPtr.h>

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		class CTimeFuncPtr : public CBaseFuncPtr
		{
		public:
			using PTR = std::function<void()>;

		private:
			//--- メンバ変数
			PTR m_Func;
			float m_fTime;
			bool m_bLoop;
		public:
			//--- メンバ関数
			CTimeFuncPtr(PTR ptr, float time = 1, bool loop = false);
			~CTimeFuncPtr();

			bool Update();

			void* GetAddress() { return &m_Func; }
		};
	}
}
#endif // !__TIME_FUNC_PTR_H__
