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
	namespace Game
	{
	}

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
			//friend class CScene;
		public:
			//--- エイリアス
			using ObjList = std::list< std::shared_ptr<CGameObject> >;
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
			using TagObjMap = std::unordered_map<std::string, gameObjWeakList>;

		private:
			ObjList m_objMgr;						// 実際の所持
			ObjList m_addObjList;					// ↑ に追加するオブジェクト格納用
			ObjList m_dontDestroyMgr;				// shared_ptrの性質を利用して、シーン遷移時に渡す非破棄リスト
			TagObjMap m_tagMap;						// tag検索用
			std::weak_ptr<CScene> m_pAffiliationScene;

		private:
			// *追加待ちリストを配列に追加する
			bool ObjectListUpdate();

			// *配列追加
			void SetGameObject(std::shared_ptr<CGameObject> obj);
			// *tagMap移動
			void TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj);
			// *@シーン取得
			std::weak_ptr<CScene> GetScene() 
			{ 
				return m_pAffiliationScene; 
			};
			
		public:
			CGameObjectManager();
			~CGameObjectManager();
			void SetAffiliationScene(std::shared_ptr<CScene> scene)
			{
				m_pAffiliationScene = scene;
			};
			// 主要処理
			void Init();
			void Uninit();
			void Update();
			void UpdateInDebug();
			void FixedUpdate();
			void AllUninit()
			{
				m_objMgr.clear();
				m_addObjList.clear();
				m_dontDestroyMgr.clear();
			}

			// 必要最低限のゲームオブジェクト作成
			void CreateBasicObject();

			// *外部でゲームオブジェクトを作りたい時に呼び出す関数
			// *処理順を考えると生成と初期化は分けたい + 配列に加えるため
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr);

			// *オブジェクトの破棄
			bool DestroyObject(std::weak_ptr<CGameObject> pObj);

			// *オブジェクト追加待ちに追加
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) 
			{
				m_addObjList.push_back(obj); 
			}

			//--- ゲッター・セッター
			// 所持リスト(引き数intでweak)
			inline ObjList GetList() 
			{
				return m_objMgr; 
			}
			// *@所持リスト(weak用)
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_objMgr)
					ret.push_back(obj);
				return ret;
			}

			// *オブジェクト上書き
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list)
			{
				m_objMgr = list;
			}

			// *@オブジェクト検索(名前)
			std::weak_ptr<CGameObject> FindGameObj(std::string name) 
			{
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) 
				{
					if ((*it).get()->GetName() == name)
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@オブジェクト検索(タグ名)
			std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag) 
			{
				if (m_tagMap.count(tag) == 0)
				{
					m_tagMap[tag] = gameObjWeakList();
					return std::weak_ptr<CGameObject>();
				}
				return m_tagMap[tag].list.begin()->lock();
			}
			// *オブジェクト検索(ﾀｸﾞｸﾗｽ)
			std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag) 
			{
				ObjList::iterator it = m_objMgr.begin();
				for (; it != m_objMgr.end(); ++it) {
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *オブジェクト検索(ﾀｸﾞｸﾗｽ)
			std::list<std::weak_ptr<CGameObject>> FindGameObjctsWithTag(std::string tag)
			{
				if (m_tagMap.count(tag) == 0)
				{
					m_tagMap[tag] = gameObjWeakList();
					return std::list<std::weak_ptr<CGameObject>>();
				}
				return m_tagMap[tag].list;
			}

			// *非破壊登録
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr) 
			{
				m_dontDestroyMgr.push_back(ptr.lock()); 
			}

			// *非破壊リストの引き渡し
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_dontDestroyMgr)
				{
					mgr->SetGameObject(obj);	// 直接追加
					mgr->DontDestroy(obj);		// 登録
					obj->SetScene(mgr->GetScene());	// TODO: DontDestroySceneを作る?
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
