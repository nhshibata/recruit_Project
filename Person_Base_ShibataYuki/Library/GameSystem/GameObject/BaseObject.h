//=========================================================
// [BaseObject.cpp]
// 基底クラス
// 作成:2022/06/07
// スマートポインタを使いやすくするために作成したｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

//--- インクルード部
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>
#include <DebugSystem/debug.h>

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
			//--- 再定義
			using Ptr = std::weak_ptr<T>;
		private:
			std::string m_objName = std::string();		// 名前
			Ptr m_spThisPtr;			// 自身のスマートポインタ

		public:
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

			// <Summary>
			// NOTE: オブジェクト、コンポーネント生成時にポインタを渡さなければいけない
			// </Summary>
			void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

			// <Summary>
			// weakPtrの取得
			// </Summary>
			Ptr GetPtr() { return m_spThisPtr; }

			// <Summary>
			// weakPtr限定
			// 配列にあるか確認
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
			bool IsPtrIn(std::list<T1> ptr, T1 obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it).lock() == obj.lock()) { return true; }		// weak
					if ((*it) == obj) { return true; }					// shared
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
			//bool SwitchDebug() { m_bDebug ^= true; return m_bDebug; }
			//bool* GetDebugAdress() { return &m_bDebug; }
			//static void SetScene(const const CScene* pScene) { m_pScene = pScene; }
#endif // BUILD_MODE
		};
	}
}

#endif // !#define __BASEOBJECT_H__

