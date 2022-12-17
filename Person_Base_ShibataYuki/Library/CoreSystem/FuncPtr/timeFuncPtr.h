//==========================================================
// [timeFuncPtr.h]
// �쐬:2022/06/23
// ���Ԍo�߂Ŋ֐������s����
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __TIME_FUNC_PTR_H__
#define __TIME_FUNC_PTR_H__

//--- �C���N���[�h��
#include <CoreSystem/FuncPtr/funcPtr.h>

namespace MySpace
{
	namespace System
	{
		//--- �N���X��`
		class CTimeFuncPtr : public CBaseFuncPtr
		{
		public:
			using PTR = std::function<void()>;

		private:
			//--- �����o�ϐ�
			PTR m_Func;
			float m_fTime;
			bool m_bLoop;
		public:
			//--- �����o�֐�
			CTimeFuncPtr(PTR ptr, float time = 1, bool loop = false);
			~CTimeFuncPtr();

			bool Update();

			void* GetAddress() { return &m_Func; }
		};
	}
}
#endif // !__TIME_FUNC_PTR_H__
