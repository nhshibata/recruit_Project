//=========================================================
// [systemBase.h]
//---------------------------------------------------------
// �쐬:2023/01/17
//---------------------------------------------------------
// Application�ɓn�����߂̊��N���X
//=========================================================

#ifndef __SYSTEM_BASE_H__
#define __SYSTEM_BASE_H__

namespace MySpace
{
	namespace System
	{
		// *@Application�ɓn�����߂̊��N���X
		class CSystemBase
		{
		public:
			CSystemBase() = default;
			virtual ~CSystemBase() = default;

#if 0
			template<class T>
			static T* Get()
			{
				static T instance;
				return &instance;
			}
#endif // 1

		};

	}
}

#endif // !__SYSTEM_BASE_H__
