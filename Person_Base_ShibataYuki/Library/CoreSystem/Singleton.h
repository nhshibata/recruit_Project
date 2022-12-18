//==========================================================
// [Singleton.h]
// 作成:2022/06/07 
// 更新:2022/07/29 ローカル静的変数にしていたが、意図しないエラーが怖いので動的方法に変更
// unityで見たシングルトンを参考に
//==========================================================

//--- インクルードガード
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <vector>

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		// 型変換のためだけのｸﾗｽ
		//class CBase
		//{
		//public:
		//	void operator=(CBase) = delete;

		//	// *@解放処理
		//	virtual void Uninit() {};
		//};

		//--- クラス定義
		template <class T>
		class CSingleton /*: public CBase*/
		{
		protected:
			CSingleton() = default;
			~CSingleton() = default;
			
		public:
			//--- メンバ関数

			// 代入禁止
			void operator=(const CSingleton<T> t) = delete;

#define LOCAL_PATTERN _DEBUG | true
#if LOCAL_PATTERN
			//--- 静的変数の使用
			static T* Get() { static T pInstance; return &pInstance; }
			// *@シングルトン破棄
			static inline void Create() {  };
			// *@シングルトン破棄
			static inline void Destroy() { }
#else
			//--- 動的に作成
		private:
			static inline T* m_pInstance = nullptr;

		public:
			// *@シングルトン取得
			static inline T* Get() { return m_pInstance; }
			// *@シングルトン破棄
			static inline void Create() { if (m_pInstance)return; m_pInstance = new T(); };
			// *@シングルトン破棄
			static inline void Destroy() { delete m_pInstance; }
#endif // LOCAL_PATTERN

		};
	}

	using MySpace::System::CSingleton;
}

#endif // !__SINGLETON_H__
