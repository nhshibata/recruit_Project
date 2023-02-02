//=========================================================
// [typeSaveManager.h]
// 作成：2022/07/10
//
// 型を保存するｸﾗｽ。ﾃﾝﾌﾟﾚｰﾄを指定することで、型の生ポインタとSPの生成を行う
// 設計上、記録するためには生成しなければならない
// あまり綺麗ではないが、基底クラスに仮想関数を定義しなければいけない
//=========================================================

#ifndef __TYPE_SAVE_MANAGER_H__
#define __TYPE_SAVE_MANAGER_H__

//--- インクルード部
#include <CoreSystem/Util/stl.h>
#include <DebugSystem/debug.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>		// gameobject用 これがないとcerealでエラーが起きる

namespace MySpace
{
	namespace Debug
	{
		//--- 前方参照
		class CTypeSaveManager;
	}
}

namespace MySpace
{
	namespace Debug
	{
		using MySpace::Game::CGameObject;

		// 基底クラス
		class CTypeSaveBase
		{
		public:
			// 後述CTypeSaveManagerが管理するためのｸﾗｽ
			CTypeSaveBase() {};
			virtual ~CTypeSaveBase() {};
#pragma warning(push)
#pragma warning(disable:4100)
			virtual std::string Set(CGameObject* obj) { return  std::string(); };
#pragma warning(pop)
		};

		// 派生ｸﾗｽ
		// 型を保存する
		template <class T1>
		class CTypeSave : public CTypeSaveBase
		{
		public:
			CTypeSave() {};
			~CTypeSave() {};

			virtual std::string Set(CGameObject* obj) { return obj->GetName(); };

			// 生ポインタ生成
			T1* CreateType() { return new T1(); }

			// SP生成
			std::shared_ptr<T1> MakeType() { return std::make_shared<T1>(); }
		};

		template <class T1>
		class CComponentTypeSave : public CTypeSave<T1>
		{
			std::string Set(CGameObject* obj)
			{
#pragma warning(suppress: 26444)
				obj->AddComponent<T1>();
				return typeid(T1).name();
			};
		};

		//--- クラス定義
		class CTypeSaveManager
		{
		private:
			//--- エイリアス
			using StockMap = std::map<std::string, CTypeSaveBase*>;
			using StockPAIR = std::pair<std::string, CTypeSaveBase*>;
			using StockSharedMap = std::map<std::string, std::shared_ptr<CTypeSaveBase>>;
			using StockSharedPair = std::pair<std::string, std::shared_ptr<CTypeSaveBase>>;

		private:
			//--- メンバ変数
			StockMap m_aStockType;

		private:
			//--- メンバ関数
			CTypeSaveManager() = default;
			~CTypeSaveManager();
		public:

			void Uninit();

			//--- ゲッター・セッター
			// *@シングルトンの取得
			static CTypeSaveManager* Get() { static CTypeSaveManager pInstance; return &pInstance; }

			// *@格納されたｸﾗｽを名前で取得
			CTypeSaveBase* GetTypeSave(std::string name);

			// *@所持しているタイプの名前を取得
			std::vector<std::string> GetTypeNameList();

			// 格納
			template <class T>
			void SetType() 
			{ 
				auto add = new CTypeSave<T>();
				m_aStockType.insert(StockPAIR(typeid(T).name(), add));
			}
			
			template <class T>
			void SetComponentSave() 
			{ 
				auto add = new CComponentTypeSave<T>();
				m_aStockType.insert(StockPAIR(typeid(T).name(), add));
			}
			
			// *@タイプを所持しているか
			bool IsType(std::string name);

			// *@SP生成
			//template <class T>
			//std::shared_ptr<T> MakeType();

			// *@生ポインタ生成
			//template <class T>
			//T* GetTypeSave();
		
		};
	}
}

//#endif // !BUILD_MODE

#endif // !__TYPE_SAVE_MANAGER_H__
