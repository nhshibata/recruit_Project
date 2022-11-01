//=====================================================
// [property.h]
// �쐬:2022/09/12
// 
// C#��property���Q�l�����׽
// cereal�������邽�߂ɕ������׽
//=====================================================

#ifndef __PROPERTY_CEREAL_H__
#define __PROPERTY_CEREAL_H__

#include <CoreSystem/property.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace System
	{
		template<class T>
		class CPropertyAll :public CProperty<T>
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(CEREAL_NVP(m_Variable)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(CEREAL_NVP(m_Variable)
				);
			}
		public:

			virtual T& Get() { return &m_Variable; }
			virtual void Set(const T value) { m_Variable = value; }
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

