//=====================================================
// [property.h]
// �쐬:2022/09/12
// 
// C#��property���Q�l�����׽
// ����֎~�ɂ��邱�ƂŉB�������͂���(public�ɂ��Ȃ��Ǝg���ɂ���)
// ����ڰĸ׽�̑��Ŏ����傾���w�肵���B�������l�������A���׽����̃A�N�Z�X���ł��Ă��܂����ߒ��~
//=====================================================

#ifndef __PROPERTY_H__
#define __PROPERTY_H__

namespace MySpace
{
	namespace System
	{
		template<class T>
		class CProperty
		{
		protected:
			T m_Variable;

		public:
			CProperty() = default;
			~CProperty() = default;

			CProperty(const CProperty<T>&) = delete;
			void operator=(CProperty<T>) = delete;
			void operator=(CProperty<T*>) = delete;
			//CProperty<T> operator;() = delete;

			virtual T& Get() { return m_Variable; }
			virtual void Set(const T value) { m_Variable = value; }

			// ����
			__declspec(property(get = Get, put = Set)) T accessor;
		};
		// �Q�b�^�[�̂�
		//template<class T>
		//class CPropertyTypeGetter : public CProperty<T>
		//{
		//private:
		//	void Set(const T value) { m_Variable = value; }
		//public:
		//	//void operator=(CProperty<T>) = delete;
		//	//void operator=(CProperty<T*>) = delete;

		//	T& Get() { return &m_Variable; }
		//};
		//// �Z�b�^�[�̂�
		//template<class T>
		//class CPropertyTypeSetter : public CProperty<T>
		//{
		//private:
		//	T& Get() { return &m_Variable; }
		//public:
		//	//void operator=(CProperty<T>) = delete;
		//	//void operator=(CProperty<T*>) = delete;

		//	void Set(const T value) { m_Variable = value; }
		//};
	}
}

#endif // !__PROPERTY_H__

