//=========================================================
// [system.h]
// 作成：2022/07/29
//----------------------
// シングルトンによる管理からアプリケーションｸﾗｽからの一括管理移行のためのｸﾗｽ
//=========================================================

#ifndef __SYSTEM_BASE_H__
#define __SYSTEM_BASE_H__

#include <Application/Application.h>
#include <Application/systemManager.h>
#include <string>

//namespace MySpace
//{
//	namespace System
//	{
//		template <typename T>
//		class CAppSystem : public CSingleton<T>
//		{
//			friend class CSingleton<T>;
//		private:
//			//--- メンバ変数
//			Application* m_pApp;
//		public:
//			//CProperty<CSystemManager*> MainSystem;
//
//		protected:
//			//--- メンバ関数
//			CAppSystem() 
//			{ 
//			};
//			//~CAppSystem() {};
//
//		public:
//			// 読み込み・書き込み処理関数
//			// コンストラクタ、あるいは任意のタイミングで呼び出す
//			virtual void LoadSystem() {};
//			virtual void SaveSystem() {};
//		};
//	}
//
//	// 型認識用 (忘れがちなため)
//	using MySpace::System::CAppSystem;
//
//}
#endif // !__SYSTEM_BASE_H__

