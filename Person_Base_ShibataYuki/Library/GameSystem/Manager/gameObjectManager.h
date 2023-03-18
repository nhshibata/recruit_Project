//==========================================================
// [gameObjectManager.h]
// 派生クラス
// 2022/06/07 作成
// ｸﾗｽ名変更するかも
//==========================================================

//--- インクルードガード
#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Util/stl.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CGameObjectManager
		{
			friend class CGameObject;
		public:
			//--- エイリアス
			using GameObjList = std::list< std::shared_ptr<CGameObject> >;
			using WeakList = std::list< std::weak_ptr<CGameObject> >;
			
			struct STGameObjWeakList 
			{
				WeakList list;
				typename std::list<std::weak_ptr<CGameObject>>::iterator FindObj(std::weak_ptr<CGameObject> obj)
				{
					for (auto it = list.begin(); it != list.end(); ++it)
					{
						if ((*it).lock() == obj.lock())
						{
							return it;
						}
					}
					return list.end();
				}
			};
			//--- エイリアス
			// *@tag検索用
			using TagObjMap = std::unordered_map<std::string, STGameObjWeakList>;

		private:
			GameObjList m_aGameObjList;					// 実際の所持
			GameObjList m_aAddObjList;					// ↑ に追加するオブジェクト格納用
			GameObjList m_aDontDestroyList;				// shared_ptrの性質を利用して、シーン遷移時に渡す非破棄リスト
			TagObjMap m_aTagMap;						// tag検索用
			std::weak_ptr<CScene> m_pAffiliationScene;  // 所属シーン

		private:
			// *@追加待ちリストを配列に追加
			bool ObjectListAddUpdate();

			// *@配列へ追加
			void SetGameObject(std::shared_ptr<CGameObject> obj);
			// *@tagMap移動
			void TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj);
			// *@所属シーン取得
			std::weak_ptr<CScene> GetScene() 
			{ 
				return m_pAffiliationScene; 
			};
			
		public:
			CGameObjectManager(std::shared_ptr<CScene> scene);
			~CGameObjectManager();
			
			//--- 主要処理
			void Init();
			void Uninit();
			void Update();
			void UpdateInDebug();
			void FixedUpdate();
			void AllUninit();
			

			// *@必要最低限のゲームオブジェクト作成
			void CreateBasicObject();

			// *@外部でゲームオブジェクトを作りたい時に呼び出す関数
			// *@処理順を考えると生成と初期化は分けたい + 配列に加えるため
			// *@引き数:CGameObject(ｺﾋﾟｰを行う)
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr);

			// *@オブジェクトの破棄
			bool DestroyObject(std::weak_ptr<CGameObject> pObj);

			// *@オブジェクト追加待ちに追加
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) 
			{
				m_aAddObjList.push_back(obj); 
			}

			//--- ゲッター・セッター
			// @所持リスト(引き数intでweak)
			inline GameObjList GetList() 
			{
				return m_aGameObjList; 
			}

			// *@オブジェクト上書き
			void SetObjList(std::list<std::shared_ptr<CGameObject>> list, bool addDrive = false);

			// *@オブジェクト検索(名前)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObj(std::string name);
			
			// *@オブジェクト検索(タグ名)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag);
			
			// *@オブジェクト検索(ﾀｸﾞｸﾗｽ)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag);
			
			// *@オブジェクト検索(ﾀｸﾞｸﾗｽ)
			_NODISCARD std::list<std::weak_ptr<CGameObject>> FindGameObjctsWithTag(std::string tag);

			// *@非破壊登録
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr)
			{
				m_aDontDestroyList.push_back(ptr.lock()); 
			}

			// *@非破壊リストの引き渡し
			void PassDontDestroyList(CGameObjectManager* mgr);
			
		};
	}
}
#endif // !__MANAGER_H__
