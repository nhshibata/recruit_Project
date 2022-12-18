//=========================================================
// [BaseObject.cpp]
//---------------------
// 作成:2022/06/07
//---------------------
// 基底クラス
// スマートポインタを使いやすくするために作成したｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

//--- インクルード部
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		template <class T>
		class CBaseObject
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_objName), CEREAL_NVP(m_spThisPtr));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_objName), CEREAL_NVP(m_spThisPtr));
			}
		public:
			//--- エイリアス
			using Ptr = std::weak_ptr<T>;

		protected:
			//--- メンバ変数
			std::string m_objName = std::string();	// 名前
		private:
			Ptr m_spThisPtr;						// 自身のスマートポインタ

		public:
			//--- メンバ関数
			CBaseObject() : m_objName(typeid(*this).name()) {};
			CBaseObject(const CBaseObject & obj)  
			{
				this->m_objName = obj.m_objName;
			};
			~CBaseObject() {};

			// *@破棄される際呼び出される
			virtual void Uninit() {}

			std::string GetName() { return m_objName; }
			void SetName(std::string name) { m_objName = name; };
			type_info& GetType() { return typeid(*this); }

			
			// NOTE: オブジェクト、コンポーネント生成時にポインタを渡さなければいけない
			// *@SP設定
			void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

			// *@weakPtr取得
			Ptr GetPtr() { return m_spThisPtr; }

			// *@weakPtr限定
			// *@配列にあるか確認
			/*template <class T2, class S>
			bool IsPtrIn(T2 ptr, S obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it) == obj) { return true; }
				}
				return false;
			}*/

			// *@list限定
			//template <class T1>
			//bool IsPtrIn(std::list<T1> ptr, T1 obj) {
			//	auto it = ptr.begin();
			//	for (; it != ptr.end(); ++it)
			//	{
			//		if ((*it).lock() == obj.lock()) { return true; }		// weak
			//		if ((*it) == obj) { return true; }					// shared
			//	}
			//	return false;
			//}

#ifdef BUILD_MODE
		public:
			// デバッグ関係
			virtual void ImGuiDebug() {};
#endif // BUILD_MODE
		};
	}
}

#endif // !#define __BASEOBJECT_H__

