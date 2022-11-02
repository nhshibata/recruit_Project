//=========================================================
// [gameobjectManager.cpp]
// 作成:2022/06/07
//---------------------------------------------------------
// ゲームオブジェクトを管理するｸﾗｽ
// 管理のみで生成は他のｸﾗｽで行う
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>

#include <GameSystem/Manager/sceneManager.h>

using namespace MySpace::Game;
//using namespace MySpace;

// コンストラクタ
CGameObjectManager::CGameObjectManager()
{
	
}
// デストラクタ
CGameObjectManager::~CGameObjectManager()
{
}
// 初期化
void CGameObjectManager::Init()
{
	for (auto & obj : m_objMgr)
	{
		obj->Init();
	}
}
// 解放
void CGameObjectManager::Uninit()
{
	// 全て除外
	for (auto & obj : m_objMgr)
	{
		obj.reset();
	}
	m_objMgr.clear();
	m_pCollisionComponent.clear();
}
// 更新
void CGameObjectManager::Update()
{
	WeakList pActiveObj(0);		// アクティブなオブジェクトを格納
	WeakList pDestoroyObj(0);	// 破棄オブジェクトを格納

	// オブジェクト更新
	// 状態確認（問題は次のフレームまでは破棄されないオブジェクトがあること?)
	for (auto & obj : m_objMgr)
	{
		// 状態により分岐
		switch (obj->GetState())
		{
		case CGameObject::ACTIVE:				// 通常の更新
			// トランスフォームの更新
			//(*it).get()->GetTransform()->Update();

			// アクティブなオブジェクトを格納
			pActiveObj.push_back(obj);
			break;
		case CGameObject::WAIT:					// 待機
			break;
		case CGameObject::DESTROY:				// 削除オブジェクト
			// 格納
			pDestoroyObj.push_back(obj);
			break;
		case CGameObject::TAKEOVER:				// 引き継ぎ待ち
			break;
		case CGameObject::MAX_OBJECT_STATE:		// 最大数
			break;
		default:
			break;
		}
	}

	// 当たり判定確認
	CollisionCheck();

	// Tweenの更新(順番検討)
	CTweenManager::Get().Update();

	//--- アクティブのものだけ
	// コンポーネントの更新
	for (auto & obj : pActiveObj)
	{
		// component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;
#ifdef _DEBUG

		[[maybe_unused]]auto name = obj.lock()->GetName();
#endif // _DEBUG

		obj.lock()->Update();
	}
	
	// 最後の方に更新したいものを更新
	for (auto & obj : pActiveObj)
	{
		// component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;
		obj.lock()->LateUpdate();
	}

	// オブジェクトの破棄
	for (auto & obj : pDestoroyObj)
	{	// FIXME: 削除されるのか?
		DestroyObject(obj);
		obj.reset();
	}

	// 追加オブジェクトの確認、追加
	ObjectListUpdate();
	
	// 配列のリセット
	pActiveObj.clear();
	pDestoroyObj.clear();
}
// 更新
void CGameObjectManager::UpdateInDebug()
{
	WeakList pDestoroyObj(0);	// 破棄オブジェクトを格納

	// 状態確認（問題は次のフレームまでは破棄されないオブジェクトがあること?)
	for (auto & obj : m_objMgr)
	{
		// 状態により分岐
		switch (obj->GetState())
		{
		case CGameObject::ACTIVE:				// 通常の更新
			// トランスフォームの更新
			obj.get()->GetTransform()->Update();
			break;
		case CGameObject::WAIT:					// 待機
			break;
		case CGameObject::DESTROY:				// 削除オブジェクト
			// 格納
			pDestoroyObj.push_back(obj);
			break;
		case CGameObject::TAKEOVER:				// 引き継ぎ待ち
			break;
		case CGameObject::MAX_OBJECT_STATE:		// 最大数
			break;
		default:
			break;
		}
	}

	// オブジェクトの破棄
	for (auto & obj : pDestoroyObj)
	{	// FIXME: 削除されるのか?
		DestroyObject(obj);
		obj.reset();
	}

	// 追加オブジェクトの確認、追加
	ObjectListUpdate();
	
	// 配列のリセット
	pDestoroyObj.clear();
}
void CGameObjectManager::FixedUpdate()
{
	// オブジェクト更新
	// コンポーネントの更新
	for (auto & obj : m_objMgr)
	{
		// component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;

		// 状態により分岐
		if (obj->GetState() == CGameObject::ACTIVE) 
		{
			obj->FixedUpdate();
		}
	}
}
// REVIEW: 改善の余地あり
void CGameObjectManager::CollisionCheck()
{
	if (m_pCollisionComponent.size() == 0)
		return;
#if 1
	//--- 当たり判定を行うｺﾝﾎﾟｰﾈﾝﾄを選別する
	COLLISION_VEC list;
	auto first_col = m_pCollisionComponent.front();
	// 最初がnullなら探索
	if (!first_col.lock())
	{
		for (auto it = m_pCollisionComponent.begin(); it != m_pCollisionComponent.end(); ++it)
		{
			if ((*it).lock())
				first_col = (*it);
		}
	}
	list.push_back(first_col);

	for (auto it = m_pCollisionComponent.begin(); it != m_pCollisionComponent.end(); ++it)
	{
		// 同一のオブジェクトは判定しない
		if (first_col.lock() == (*it).lock())
			continue;
		// 破棄されていないか確認
		if (!(*it).lock()->GetOwner()->IsActive() || !(*it).lock())
		{			
			continue;
		}
#ifdef _DEBUG
		auto name = first_col.lock()->GetOwner()->GetName();
		auto flg = first_col.lock()->GetOwner()->GetState();
		auto other_name = (*it).lock()->GetOwner()->GetName();
		auto other_flg = (*it).lock()->GetOwner()->GetState();
		// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
		bool res = first_col.lock()->HitCheckPtr((*it).lock().get());
		list.push_back((*it));
#else
		// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
		first_col.lock()->HitCheckPtr((*it).lock().get());
		list.push_back((*it));	// これ忘れると意味ない(やらかし1)
#endif
		
	}
	// 最初以外
	for (int cnt = 1; cnt < static_cast<int>(list.size()); ++cnt) 
	{
		for (int otherCnt = 0; otherCnt < static_cast<int>(list.size()); ++otherCnt)
		{
			if (list[cnt].lock() == list[otherCnt].lock())
				continue;
#ifdef _DEBUG
			auto name = list[cnt].lock()->GetOwner()->GetName();
			auto flg = list[cnt].lock()->GetOwner()->GetState();
			auto other_name = list[otherCnt].lock()->GetOwner()->GetName();
			auto other_flg = list[otherCnt].lock()->GetOwner()->GetState();
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			bool res = list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#else
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#endif // _DEBUG
		}
	}

	// 離れたオブジェクトを確認
	// 配列からの除外
	for (int cnt = 0; cnt < static_cast<int>(m_pCollisionComponent.size()); ++cnt)
	{
		if (!m_pCollisionComponent[cnt].lock()->IsActive())
			continue;
		if (!m_pCollisionComponent[cnt].lock())
		{	// 破棄(最後尾を代入)
			m_pCollisionComponent[cnt] = m_pCollisionComponent.back();
			m_pCollisionComponent.pop_back();
			--cnt;	// 仕切り直し
			continue;
		}
		m_pCollisionComponent[cnt].lock()->ExitTell();
	}

	return;
#endif // 0

#pragma region OLD
	for (auto & col : m_pCollisionComponent)
	{
		// 判定状態か確認
		if (!col.lock())
			continue;
		// 判定状態か確認
		if (!col.lock()->GetOwner()->IsActive())
			continue;

		for (auto & other : m_pCollisionComponent)
		{
			// 同一のオブジェクトは判定しない
			// 破棄されていないか確認
			if (col.lock() == other.lock() || !other.lock()->GetOwner()->IsActive() || !other.lock())
			{
				continue;
			}
#ifdef _DEBUG
			auto name = col.lock()->GetOwner()->GetName();
			auto flg = col.lock()->GetOwner()->GetState();
			auto other_name = other.lock()->GetOwner()->GetName();
			auto other_flg = other.lock()->GetOwner()->GetState();
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			bool res = col.lock()->HitCheckPtr(other.lock().get());
#else
			// 自身のコリジョンｺﾝﾎﾟｰﾈﾝﾄを引き渡す
			col.lock()->HitCheckPtr(other.lock().get());
#endif // _DEBUG
		}// other
	}// this

	// 離れたオブジェクトを確認
	// 配列からの除外
	for (int cnt = 0; cnt < static_cast<int>(m_pCollisionComponent.size()); ++cnt)
	{	
		if (!m_pCollisionComponent[cnt].lock()->IsActive())
			continue;
		if (!m_pCollisionComponent[cnt].lock())
		{	// 破棄(最後尾を代入)
			m_pCollisionComponent[cnt] = m_pCollisionComponent.back();
			m_pCollisionComponent.pop_back();
			--cnt;	// 仕切り直し
			continue;
		}
		m_pCollisionComponent[cnt].lock()->ExitTell();
	}
	// 配列を空に
	//m_pCollisionComponent.clear();
#pragma endregion
}
// オブジェクトの追加
bool CGameObjectManager::ObjectListUpdate()
{
	// 追加オブジェクトが空でない時
	// 途中追加のオブジェがある場合のためwhile
	while(m_addObjList.size() != 0)
	{
		auto addList = m_addObjList;	// 受け取る
		m_addObjList.clear();			// 配列を空に

		// 格納し、処理を呼び出す
		for (auto & addObj : addList)
		{
			if (addObj)
			{
				addObj->Awake();
				SetGameObject(addObj);
			}
		}
		for (auto & addObj : addList)
		{
#ifdef _DEBUG
			auto name = addObj->GetName();
#endif // !_DEBUG

			if (addObj)
			{
				addObj->Init();
			}
		}
	}
	
	return false;
}
// 描画に必要なオブジェクトの作成セット
void CGameObjectManager::CreateBasicObject()
{
	std::shared_ptr<CGameObject> pObj = CreateGameObject();
	// 必須
	// ｶﾒﾗ
	pObj->AddComponent<CCamera>();
	// ライト
	pObj = CreateGameObject();
	pObj->AddComponent<CDirectionalLight>();
}
// *配列追加
void CGameObjectManager::SetGameObject(std::shared_ptr<CGameObject> obj)
{
	if (!obj.get()->IsPtrIn <ObjList, std::shared_ptr<CGameObject>>(m_objMgr, obj))
	{
		m_objMgr.push_back(obj);
		TagMove(obj->GetTag(),obj);
	}
}
void CGameObjectManager::TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj)
{
	// 同一なら抜ける
	if (obj.lock()->GetTag() == NextTag)
		return;
	
	auto list = m_tagMap[obj.lock()->GetTag()];
	auto it = list.FindObj(obj.lock());
	// 現在のtagから除外
	if(it != list.list.end())
		m_tagMap[obj.lock()->GetTag()].list.erase(it);
	
	// 変更後のtagへ移動
	m_tagMap[NextTag].list.push_back(obj);
}
std::shared_ptr<CGameObject> CGameObjectManager::CreateGameObject(CGameObject* pObj)
{
	std::shared_ptr<CGameObject> spObj;
	// ｺﾋﾟｰ
	if (pObj)
	{
		spObj = std::make_shared<CGameObject>(*pObj);
	}
	else
	{
		spObj = std::make_shared<CGameObject>();
	}
	
	// 自分の所属シーンを教える
	spObj.get()->SetScene(m_pAffiliationScene);

	// 自身のweakPtrを渡す
	spObj.get()->SetPtr(spObj);

	TagMove("Default", spObj);

	// 初期名
	if(pObj->GetName().empty())
		spObj->SetName(std::string("GameObj_" + std::to_string(static_cast<int>(m_objMgr.size() + m_addObjList.size()) + 1)));
	
	spObj.get()->Awake();	// 実質OnCreateな気がする
	AddGameObject(spObj);	// 追加待ちリストに追加

	return spObj;
};

bool CGameObjectManager::DestroyObject(std::weak_ptr<CGameObject> pObj)
{
	// 検索
	auto it = std::find(m_objMgr.begin(), m_objMgr.end(), pObj.lock());
	if (it == m_objMgr.end())
		return false;	// 配列に居ない
	m_objMgr.erase(it);

	// tag管理からも除外
	auto weakMap = m_tagMap[pObj.lock()->GetTag()];
	auto mapIt = weakMap.FindObj(pObj.lock());
	if (mapIt != weakMap.list.end())
		weakMap.list.erase(mapIt);

	// 非破壊検索
	if (std::find(m_dontDestroyMgr.begin(), m_dontDestroyMgr.end(), pObj.lock()) != m_dontDestroyMgr.end())
		m_dontDestroyMgr.erase(it);	// FIXME: 正しく消去されるか要確認

	// 明示的な破棄
	pObj.lock().reset();
	
	// 監視対象が残っている想定しないエラー
	if (pObj.lock()) 
		return false; 	
	return true;
}