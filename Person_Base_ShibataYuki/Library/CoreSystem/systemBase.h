//=========================================================
// [systemBase.h]
//---------------------------------------------------------
// 作成:2023/01/17
//---------------------------------------------------------
// Applicationに渡すための基底クラス
//=========================================================

#ifndef __SYSTEM_BASE_H__
#define __SYSTEM_BASE_H__

namespace MySpace
{
	namespace System
	{
		// *@Applicationに渡すための基底クラス
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
