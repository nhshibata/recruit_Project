//=====================================================
// [property.h]
// 作成:2022/09/12
// 
// C#のpropertyを参考したｸﾗｽ
// 代入禁止にすることで隠蔽化をはかる(publicにしないと使いにくい)
// ﾃﾝﾌﾟﾚｰﾄｸﾗｽの第二で持ち主だけ指定した隠蔽化を考えたが、同ｸﾗｽからのアクセスができてしまうため中止
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

			// ｱｸｾｻ
			__declspec(property(get = Get, put = Set)) T accessor;
		};
		// ゲッターのみ
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
		//// セッターのみ
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

