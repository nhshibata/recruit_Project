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
#include <CoreSystem/Singleton.h>
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
			virtual void Set(CGameObject* obj) {};
		};

		// 派生ｸﾗｽ
		// 型を保存する
		template <class T1>
		class CTypeSave : public CTypeSaveBase
		{
		public:
			CTypeSave() {};
			~CTypeSave() {};

			virtual void Set(CGameObject* obj) {};

			// 生ポインタ生成
			T1* CreateType() { return new T1(); }

			// SP生成
			std::shared_ptr<T1> MakeType() { return std::make_shared<T1>(); }
		};

		template <class T1>
		class CComponentTypeSave : public CTypeSave<T1>
		{
			void Set(CGameObject* obj)
			{
				obj->AddComponent<T1>();
			};
		};

		//--- クラス定義
		class CTypeSaveManager : public CSingleton<CTypeSaveManager>
		{
			friend class CSingleton<CTypeSaveManager>;
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
			CTypeSaveManager() {};

		public:
			~CTypeSaveManager();
			void Uninit();

			//--- ゲッター・セッター

			// *@格納されたｸﾗｽを名前で取得
			CTypeSaveBase* GetTypeSave(std::string name) { return m_aStockType[name]; }

			// *@所持しているタイプの名前を取得
			std::vector<std::string> GetTypeNameList();

			// *@生ポインタ生成
			template <class T>
			T* GetTypeSave();

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
			
			// *@SP生成
			template <class T>
			std::shared_ptr<T> MakeType();

			// *@タイプを所持しているか
			bool IsType(std::string name);

		};
	}
}

//#endif // !BUILD_MODE

#endif // !__TYPE_SAVE_MANAGER_H__
