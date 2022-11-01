//==========================================================
// [funcPtr.h]
// �쐬:2022/06/17
// �쐬:2022/08/25 �߂�l�A���������w�肵���֐��|�C���^���\�z�ł���׽�쐬
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FUNC_PTR_H__
#define __FUNC_PTR_H__

//--- �C���N���[�h��
#include <functional>

namespace MySpace
{
	namespace System
	{
		// �z��ŊǗ����邽�߂̊֐��|�C���^�����x�[�X�׽
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
		// @�߂�l:void*�^
		// @������:void�^
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
		// @�ϒ��e���v���[�g(��������:�߂�l�^, ������:�������^
		// @�߂�l:void,int��
		// @������:void,int�����w��
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
			// void Bind() { std::bind(&�֐�, ptr, std::placeholders(������) ); }
		};

	}
}
#endif // !__FUNC_PTR_H__