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
			
			struct gameObjWeakList 
			{
				WeakList list;
				typename std::list<std::weak_ptr<CGameObject>>::iterator FindObj(std::weak_ptr<CGameObject> obj)
				{
					auto it = list.begin();
					for (; it != list.end(); ++it)
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
			using TagObjMap = std::unordered_map<std::string, gameObjWeakList>;

		private:
			GameObjList m_aGameObjList;					// 実際の所持
			GameObjList m_aAddObjList;					// ↑ に追加するオブジェクト格納用
			GameObjList m_aDontDestroyList;				// shared_ptrの性質を利用して、シーン遷移時に渡す非破棄リスト
			TagObjMap m_aTagMap;						// tag検索用
			std::weak_ptr<CScene> m_pAffiliationScene;

		private:
			// *@追加待ちリストを配列に追加
			bool ObjectListUpdate();

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
			void AllUninit()
			{
				m_aGameObjList.clear();
				m_aAddObjList.clear();
				m_aDontDestroyList.clear();
			}

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
			// *@所持リスト(weak用)
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_aGameObjList)
					ret.push_back(obj);
				return ret;
			}

			// *@オブジェクト上書き
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list)
			{
				m_aGameObjList = list;
			}

			// *@オブジェクト検索(名前)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObj(std::string name)
			{
				for (GameObjList::iterator it = m_aGameObjList.begin(); it != m_aGameObjList.end(); ++it) 
				{
					if ((*it).get()->GetName() == name)
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@オブジェクト検索(タグ名)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag)
			{
				if (m_aTagMap.count(tag) == 0)
				{
					//m_aTagMap[tag] = gameObjWeakList();
					return std::weak_ptr<CGameObject>();
				}
				return m_aTagMap[tag].list.begin()->lock();
			}
			// *@オブジェクト検索(ﾀｸﾞｸﾗｽ)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag)
			{
				GameObjList::iterator it = m_aGameObjList.begin();
				for (; it != m_aGameObjList.end(); ++it)
				{
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@オブジェクト検索(ﾀｸﾞｸﾗｽ)
			_NODISCARD std::list<std::weak_ptr<CGameObject>> FindGameObjctsWithTag(std::string tag)
			{
				if (m_aTagMap.count(tag) == 0)
				{
					m_aTagMap[tag] = gameObjWeakList();
					return std::list<std::weak_ptr<CGameObject>>();
				}
				return m_aTagMap[tag].list;
			}

			// *@非破壊登録
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr)
			{
				m_aDontDestroyList.push_back(ptr.lock()); 
			}

			// *@非破壊リストの引き渡し
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_aDontDestroyList)
				{
					mgr->SetGameObject(obj);		// 直接追加
					mgr->DontDestroy(obj);			// 登録
					obj->SetScene(mgr->GetScene());	// TODO: DontDestroySceneを作る?
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
