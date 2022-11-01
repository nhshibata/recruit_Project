//=========================================================
// [BaseComponent.h]
// 基底クラス
// 作成:2022/06/07
// スマートポインタを使いやすくするために作成したｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

//--- インクルード部
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>
#include <DebugSystem/debug.h>

namespace MySpace
{
	namespace Game
	{
		class CBaseComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				//archive(CEREAL_NVP(m_spThisPtr));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				//archive(CEREAL_NVP(m_spThisPtr));
			}

		public:
			CBaseComponent() { };
			~CBaseComponent() {};

			// *@破棄される際呼び出される
			virtual void Uninit() {}

			inline std::string GetName() { return typeid(*this).name(); }
			const type_info& GetType() { return typeid(*this); }

			// <Summary>
			// weakPtr限定
			// </Summary>
			template <class T2, class S>
			bool IsPtrIn(T2 ptr, S obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it) == obj) { return true; }
				}
				return false;
			}

			// <Summary>
			// list限定
			// </Summary>
			template <class T1>
			_NODISCARD 
				typename auto FindIterator(std::vector<std::weak_ptr<T1>> ptr, std::weak_ptr<T1> obj)
			{
				for (auto it = ptr.begin(); it != ptr.end(); ++it)
				{
					if ((*it).lock() == obj.lock())
					{
						return it;
					}
				}
				return ptr.end();
			}
			// *ptrが入っていたらtrue
			template <class T1>
			bool IsPtrIn(std::vector<std::weak_ptr<T1>> ptr, std::weak_ptr<T1> obj)
			{
				auto it = FindIterator<T1>(ptr, obj);
				return (it != ptr.end());
			}
			
			// <Summary>
			// list限定
			// </Summary>
			template <class T1>
			bool IsPtrIn(std::list<T1> ptr, T1 obj) {
				for (auto it = ptr.begin(); it != ptr.end(); ++it)
				{
					if ((*it).lock() == obj.lock()) { return true; }
				}
				return false;
			}

#ifdef BUILD_MODE
		protected:
			//bool m_bDebug;
		public:
			// デバッグ関係
			virtual void ImGuiDebug() {};
			//void SetDebug(bool flg) { m_bDebug = flg; }
			//bool GetDebug() { return m_bDebug; }
			//bool *GetDebugAdress() { return &m_bDebug; }

#endif // BUILD_MODE

		};
	}
}

#endif // __BASE_COMPONENT_H__