//==========================================================
// [funcPtrMnagaer.h]
// 作成:2022/06/23
// 更新:2022/08/20 シーン用は分割
//----------------------------------------------------------
// funcPtrｸﾗｽはこのｸﾗｽで作られ、管理される(シーン内の処理は別)
//==========================================================

//--- インクルードガード
#ifndef __FUNC_PTR_MANAGER_H__
#define __FUNC_PTR_MANAGER_H__

//--- インクルード部
#include <vector>
#include <memory>
#include <CoreSystem/FuncPtr/funcPtr.h>
#include <CoreSystem/FuncPtr/timeFuncPtr.h>
#include <CoreSystem/Singleton.h>
#include <DebugSystem/debug.h>

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		class CFuncManager : public CSingleton<CFuncManager>
		{
			friend class CSingleton<CFuncManager>;
		private:
			using FUNC_VEC = std::vector<std::shared_ptr<CBaseFuncPtr>>;
			FUNC_VEC m_FuncPtr;
			//FUNC_VEC m_ReleaseFunc;

			bool m_debug;

			CFuncManager() {};
		public:
			void Update();
			void Uninit();

			//void UninitFunc();

			// 戻り値がポインタ型
			void RequestFunc(CFuncPtr::PTR func);

			//void RequestUninitFunc(CTimeFuncPtr::PTR func);

			// 戻り値がvoid
			void RequestFunc(CTimeFuncPtr::PTR func, float time = 1, bool loop = false);

			bool StopFunc(void* ptr);

#ifdef BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

			bool* IsDebug() { return &m_debug; }
		};
	}
}
#endif // !__FUNC_PTR_MANAGER_H__
