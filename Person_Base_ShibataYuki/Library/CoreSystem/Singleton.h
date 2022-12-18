//==========================================================
// [Singleton.h]
// �쐬:2022/06/07 
// �X�V:2022/07/29 ���[�J���ÓI�ϐ��ɂ��Ă������A�Ӑ}���Ȃ��G���[���|���̂œ��I���@�ɕύX
// unity�Ō����V���O���g�����Q�l��
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <vector>

namespace MySpace
{
	namespace System
	{
		//--- �N���X��`
		// �^�ϊ��̂��߂����̸׽
		//class CBase
		//{
		//public:
		//	void operator=(CBase) = delete;

		//	// *@�������
		//	virtual void Uninit() {};
		//};

		//--- �N���X��`
		template <class T>
		class CSingleton /*: public CBase*/
		{
		protected:
			CSingleton() = default;
			~CSingleton() = default;
			
		public:
			//--- �����o�֐�

			// ����֎~
			void operator=(const CSingleton<T> t) = delete;

#define LOCAL_PATTERN _DEBUG | true
#if LOCAL_PATTERN
			//--- �ÓI�ϐ��̎g�p
			static T* Get() { static T pInstance; return &pInstance; }
			// *@�V���O���g���j��
			static inline void Create() {  };
			// *@�V���O���g���j��
			static inline void Destroy() { }
#else
			//--- ���I�ɍ쐬
		private:
			static inline T* m_pInstance = nullptr;

		public:
			// *@�V���O���g���擾
			static inline T* Get() { return m_pInstance; }
			// *@�V���O���g���j��
			static inline void Create() { if (m_pInstance)return; m_pInstance = new T(); };
			// *@�V���O���g���j��
			static inline void Destroy() { delete m_pInstance; }
#endif // LOCAL_PATTERN

		};
	}

	using MySpace::System::CSingleton;
}

#endif // !__SINGLETON_H__
