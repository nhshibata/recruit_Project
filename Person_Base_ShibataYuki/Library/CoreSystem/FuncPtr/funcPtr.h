//==========================================================
// [funcPtr.h]
// 作成:2022/06/17
// 作成:2022/08/25 戻り値、引き数を指定した関数ポインタを構築できるｸﾗｽ作成
//==========================================================

//--- インクルードガード
#ifndef __FUNC_PTR_H__
#define __FUNC_PTR_H__

//--- インクルード部
#include <functional>

namespace MySpace
{
	namespace System
	{
		// 配列で管理するための関数ポインタ所持ベースｸﾗｽ
		class CBaseFuncPtr
		{
		public:

		public:
			CBaseFuncPtr() = default;
			virtual ~CBaseFuncPtr() = default;
			virtual bool Update() { return true; };
			//void SetFunc(PTR ptr) { m_Func = ptr; };
			virtual void* GetAddress() = 0;
		};
		// @戻り値:void*型
		// @引き数:void型
		class CFuncPtr : public CBaseFuncPtr
		{
		public:
			//using VoidPtr = std::function<void()>;
			using PTR = std::function<void*()>;

		private:
			PTR m_Func;

		public:
			CFuncPtr(PTR ptr);
			~CFuncPtr();
			bool Update();

			virtual void* GetAddress() { return &m_Func; };
		};
		// @可変長テンプレート(第一引き数:戻り値型, 引き数:引き数型
		// @戻り値:void,int等
		// @引き数:void,int複数指定
		template<class T, class... Args>
		class CFuncPtrTemplate : public CBaseFuncPtr
		{
		public:
			//using PTR = std::function<T(Args...)>;
			//typedef void (T::*Func)(void);
		private:
			std::function<T(Args...)> m_Func;

		public:
			CFuncPtrTemplate() {};
			CFuncPtrTemplate(std::function<T(Args...)> ptr) { m_Func = ptr; };
			~CFuncPtrTemplate() {};
			//bool Update() { return true; };

			void SetFunc(std::function<T(Args...)> ptr) { m_Func = ptr; }
			virtual void Call(Args... args) { m_Func(args...); }
			virtual void* GetAdress() { return &m_Func; };
			// void Bind() { std::bind(&関数, ptr, std::placeholders(引き数) ); }
		};

	}
}
#endif // !__FUNC_PTR_H__