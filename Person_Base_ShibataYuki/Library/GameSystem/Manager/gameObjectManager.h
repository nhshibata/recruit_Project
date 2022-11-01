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

#pragma region _forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CCollision;
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
		public:
			using ObjList = std::list< std::shared_ptr<CGameObject> >;
			using WeakList = std::list< std::weak_ptr<CGameObject> >;
			using COLLISION_VEC = std::vector<std::weak_ptr<CCollision>>;	// 途中破棄された場合、参照しないためweak

		private:
			COLLISION_VEC m_pCollisionComponent;	// 当たり判定ｸﾗｽ
			ObjList m_objMgr;
			ObjList m_addObjList;					// 追加オブジェクト格納用
			ObjList m_dontDestroyMgr;				// shared_ptrの性質を利用して、シーン遷移時に渡す非破棄リスト
			
		private:
			// *追加待ちリストを配列に追加する
			bool ObjectListUpdate();
			// *当たり判定用関数
			void CollisionCheck();
			// *配列追加
			void SetGameObject(std::shared_ptr<CGameObject> obj) 
			{
				if (!obj.get()->IsPtrIn <ObjList, std::shared_ptr<CGameObject>>(m_objMgr, obj))
				{
					m_objMgr.push_back(obj); 
				}
			}

		public:
			CGameObjectManager();
			~CGameObjectManager();

			void Init();
			void Uninit();
			void Update();
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
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr)
			{
				std::shared_ptr<CGameObject> spObj;
				if (pObj){
					spObj = std::make_shared<CGameObject>(*pObj);
				}
				else {
					spObj = std::make_shared<CGameObject>();
				}
				// 自身のweakPtrを渡す
				spObj.get()->SetPtr(spObj);
				// 初期名
				spObj->SetName(std::string("GameObj_"+ std::to_string(static_cast<int>(m_objMgr.size() + m_addObjList.size()) + 1)));
				spObj.get()->Awake();	// 実質OnCreateな気がする
				AddGameObject(spObj);	// 追加待ちリストに追加
				//spObj.get()->StartUp();
				//SetGameObject(spObj);
				return spObj;
			};

			// *オブジェクトの破棄
			bool DestroyObject(std::weak_ptr<CGameObject> pObj)
			{
				// 検索
				auto it = std::find(m_objMgr.begin(), m_objMgr.end(), pObj.lock());
				if (it == m_objMgr.end()) return false;	// 配列に居ない
				m_objMgr.erase(it);

				if(auto dontIt = std::find(m_dontDestroyMgr.begin(), m_dontDestroyMgr.end(), pObj.lock()) ;
						dontIt != m_dontDestroyMgr.end())
					m_dontDestroyMgr.erase(it);	// FIXME: 正しく消去されるか要確認

				pObj.lock().reset();
				if (pObj.lock()) { return false; }	// 監視対象が残っている想定しないエラー
				return true;
			}

			// *当たり判定依頼
			inline void SetColComponent(std::weak_ptr<CCollision> com) { m_pCollisionComponent.push_back(com); }

			// *オブジェクト追加待ちに追加
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) { m_addObjList.push_back(obj); }

			// ゲッター・セッター
			inline ObjList GetList() { return m_objMgr; }
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_objMgr)ret.push_back(obj);
				return ret;
			}

			// *オブジェクト上書き
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list) { m_objMgr = list; }

			// *オブジェクト検索(名前)
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
			// *オブジェクト検索(タグ名)
			std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag) 
			{
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) 
				{
					if ((*it).get()->GetTagPtr()->Compare(tag))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *オブジェクト検索(ﾀｸﾞｸﾗｽ)
			std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag) {
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
			std::list<std::weak_ptr<CGameObject>> FindGameObjs(CTag tag) {
				std::list<std::weak_ptr<CGameObject>> list(0);
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) {
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						list.push_back((*it));
					}
				}
				return list;
			}

			// *非破壊登録
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr) { m_dontDestroyMgr.push_back(ptr.lock()); }

			// *非破壊リストの引き渡し
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_dontDestroyMgr)
				{
					mgr->SetGameObject(obj);	// 直接追加
					mgr->DontDestroy(obj);		// 登録
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
