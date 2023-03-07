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
			// *@std::functionは型以外は応用が利く
			// *@関数ポインタ:ﾃﾝﾌﾟﾚｰﾄｸﾗｽで指定
			// *@引き数:可変長引き数

			// 引き数決定クラス
			template <class... Args>
			class CFunctionBinder 
			{
			protected:
				//--- メンバ変数
				std::function<void*(Args...)> m_Func;
			public:
				// *@コンストラクタ
				CFunctionBinder(std::function<void*(Args...)> func)
					:m_Func(func)
				{}

				// *@関数呼び出し
				virtual void Call(Args... args) 
				{
					m_Func(args...);
				}
				// *@アドレス取得
				void* GetAdress() { return &m_Func; }
			};
			// 関数型決定クラス(戻り値void*固定)
			template <class T, class... Args>
			class CFunctionBinderCall : public CFunctionBinder<Args...>
			{
			public:
				// *@コンストラクタ
				CFunctionBinderCall(std::function<void*(Args...)> func)
					:CFunctionBinder<Args...>(func)
				{}
			};
			
		public:
			//--- 定義
			using PTR = std::function<void*(void)>;
			
		private:
			//--- メンバ変数
			std::vector<CFunctionBinder<CScene*,CScene*>> m_pChangeFunc;
			std::vector<CFunctionBinder<CScene*,int>> m_pLoadFunc;
			std::vector<CFunctionBinder<CScene*>> m_pUnloadFunc;

		public:
			//--- メンバ関数
			CSceneTransitionDetection();
			~CSceneTransitionDetection();

			// *@登録された関数呼び出し
			// *@シーン遷移
			void Call(CScene* prev, CScene* next);

			// *@登録された関数の呼び出し
			// *@シーン読み込み時
			void Call(CScene* scene, int);

			// *@登録された関数の呼び出し
			// *@シーン破棄時
			void Call(CScene* scene);				

			// *@シーン切替時
			// *@ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Changed(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_pChangeFunc.push_back(CFunctionBinderCall<T, CScene*, CScene*>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}

			// *@シーン読み込み時
			// *@ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Loaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_pLoadFunc.push_back(CFunctionBinderCall<T, CScene*, int>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}

			// *@シーン破棄時
			// *@ﾃﾝﾌﾟﾚｰﾄｸﾗｽ:渡したい関数の型
			template <class T>
			void Unloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_pUnloadFunc.push_back(CFunctionBinderCall<T, CScene*>(std::bind(func, ptr, std::placeholders::_1)));
			}

			// *@登録した関数除外
			bool StopFunc(void* ptr);		
			
		};
	}
}

#endif // !__SCENE_TRANSITIONDETECTION_H__

