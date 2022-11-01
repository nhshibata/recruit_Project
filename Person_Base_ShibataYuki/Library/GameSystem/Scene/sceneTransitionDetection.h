//==========================================================
// [sceneTransitionDetection.h]
// 作成:2022/08/20
//----------------------------------------------------------
// シーンから呼び出したい関数ポインタを登録するｸﾗｽ
//==========================================================

//--- インクルードガード
#ifndef __SCENE_TRANSITION_DETECTION_H__
#define __SCENE_TRANSITION_DETECTION_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/FuncPtr/funcPtr.h>

namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}

namespace MySpace
{
	namespace SceneManager
	{
		// *関数ポインタ管理ｸﾗｽ
		class CSceneTransitionDetection
		{
		private:
			// *std::functionは型以外は応用が利く
			// *関数ポインタ:ﾃﾝﾌﾟﾚｰﾄｸﾗｽで指定
			// *引き数:可変長引き数

			// 引き数決定
			template <class... Args>
			class CReturnVoidFunc 
			{
			protected:
				using Ptr = std::function<void*(Args...)>;
				// 関数ポインタ
				std::function<void*(Args...)> m_Func;
			public:
				CReturnVoidFunc(std::function<void*(Args...)> func) { m_Func = func; }

				virtual void Call(Args... args) 
				{
					m_Func(args...);
				}
				//virtual void* GetAdress() { return nullptr; };
				void* GetAdress() { return &m_Func; }
			};
			// 関数型決定(戻り値void*固定)
			template <class T, class... Args>
			class CCallFunc : public CReturnVoidFunc<Args...>
			{
			public:
				// 関数ポインタ型
				typedef void* (T::*Func)(Args... args);
				
			public:
				CCallFunc(std::function<void*(Args...)> func):CReturnVoidFunc<Args...>(func){ }
				//void Call(Args... args) 
			};
			
		public:
			enum class EFuncType
			{
				CHANGED,
				LOADED,
				UNLOADED,
			};
			using PTR = std::function<void*(void)>;
			/*using ChangePtr = std::function<void*(CScene*, CScene*)>;
			using LoadPtr = std::function<void*(CScene*, int)>;
			using UnloadPtr = std::function<void*(CScene*)>;*/
			
		private:
			std::vector<CReturnVoidFunc<CScene*,CScene*>> m_pChangeFunc;
			std::vector<CReturnVoidFunc<CScene*,int>> m_pLoadFunc;
			std::vector<CReturnVoidFunc<CScene*>> m_pUnloadFunc;

		public:
			CSceneTransitionDetection();
			~CSceneTransitionDetection();

			void Call(CScene* prev, CScene* next);	// *登録された関数の呼び出し
			void Call(CScene* scene, int);			// *登録された関数の呼び出し
			void Call(CScene* scene);				// *登録された関数の呼び出し

			// *シーン切替時
			// *ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Changed(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_pChangeFunc.push_back(CCallFunc<T, CScene*, CScene*>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}
			// *シーン読み込み時
			// *ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Loaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_pLoadFunc.push_back(CCallFunc<T, CScene*, int>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}
			// *シーン破棄時
			// *ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Unloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_pUnloadFunc.push_back(CCallFunc<T, CScene*>(std::bind(func, ptr, std::placeholders::_1)));
			}
			// *登録した関数を除外
			bool StopFunc(void* ptr);		
			
		};
	}
}

#endif // !__SCENE_TRANSITIONDETECTION_H__

