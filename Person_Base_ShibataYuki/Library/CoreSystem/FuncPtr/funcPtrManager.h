//==========================================================
// [funcPtrMnagaer.h]
// �쐬:2022/06/23
// �X�V:2022/08/20 �V�[���p�͕���
//----------------------------------------------------------
// funcPtr�׽�͂��̸׽�ō���A�Ǘ������(�V�[�����̏����͕�)
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FUNC_PTR_MANAGER_H__
#define __FUNC_PTR_MANAGER_H__

//--- �C���N���[�h��
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
		//--- �N���X��`
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

			// �߂�l���|�C���^�^
			void RequestFunc(CFuncPtr::PTR func);

			//void RequestUninitFunc(CTimeFuncPtr::PTR func);

			// �߂�l��void
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
