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
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CGameObjectManager::CGameObjectManager(std::shared_ptr<CScene> scene)
{
	this->m_pAffiliationScene = scene;
}

//==========================================================
// デストラクタ
//==========================================================
CGameObjectManager::~CGameObjectManager()
{
}

//==========================================================
// 初期化
//==========================================================
void CGameObjectManager::Init()
{
	for (auto & obj : m_aGameObjList)
	{
		obj->Init();
	}
}

//==========================================================
// 解放
//==========================================================
void CGameObjectManager::Uninit()
{
	// 全て除外
	for (auto & obj : m_aGameObjList)
	{
		obj->Uninit();
		obj.reset();
	}
	m_aGameObjList.clear();
	m_aAddObjList.clear();
	m_aTagMap.clear();
}

//==========================================================
// 更新
//==========================================================
void CGameObjectManager::Update()
{
	WeakList pActiveObj(0);		// アクティブなオブジェクトを格納
	WeakList pDestoroyObj(0);	// 破棄オブジェクトを格納

	//--- オブジェクト更新
	// 状態確認（問題は次のフレームまでは破棄されないオブジェクトがあること?)
	for (auto & obj : m_aGameObjList)
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

	//--- オブジェクトの破棄
	for (auto & obj : pDestoroyObj)
	{
		DestroyObject(obj);
		obj.reset();
	}
	// 配列のリセット
	pDestoroyObj.clear();

	//--- アクティブだけ
	// コンポーネントの更新
	for (auto & obj : pActiveObj)
	{
		//--- component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;

#ifdef _DEBUG
		[[maybe_unused]]auto name = obj.lock()->GetName();
#endif // _DEBUG

		obj.lock()->Update();
	}
	
	// 最後の方に更新したいものを更新
	for (auto & obj : pActiveObj)
	{
		//--- component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;
		obj.lock()->LateUpdate();
	}
	// 配列のリセット
	pActiveObj.clear();

	// 追加オブジェクトの確認、追加
	if (m_aAddObjList.size() != 0)
		ObjectListUpdate();
	
}

//==========================================================
// 更新
//==========================================================
void CGameObjectManager::UpdateInDebug()
{
	WeakList pDestoroyObj(0);	// 破棄オブジェクトを格納

	// 状態確認（問題は次のフレームまでは破棄されないオブジェクトがあること?)
	for (auto & obj : m_aGameObjList)
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

	// 配列のリセット
	pDestoroyObj.clear();

	// 追加オブジェクトの確認、追加
	if(m_aAddObjList.size() != 0)
		ObjectListUpdate();	
}

//==========================================================
// 固定更新
//==========================================================
void CGameObjectManager::FixedUpdate()
{
	// オブジェクト更新
	// コンポーネントの更新
	for (auto & obj : m_aGameObjList)
	{
		//--- component内でシーンが破棄された場合、処理を抜ける
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;

		if (obj->GetState() == CGameObject::ACTIVE) 
		{
			obj->FixedUpdate();
		}
	}
}

//==========================================================
// オブジェクト追加
//==========================================================
bool CGameObjectManager::ObjectListUpdate()
{
	// 追加オブジェクトが空でない時
	// 途中追加のオブジェがある場合のためwhile
	while(m_aAddObjList.size() != 0)
	{
		auto addList = m_aAddObjList;	// 受け取る
		m_aAddObjList.clear();			// 配列を空に

		// 格納し、処理を呼び出す
		for (auto & addObj : addList)
		{
			if (addObj)
			{
				addObj->Awake();
				SetGameObject(addObj);
			}
		}
		
		//--- 初期化呼び出し
		for (auto & addObj : addList)
		{

#ifdef _DEBUG
			auto name = addObj->GetName();
#endif // !_DEBUG


			addObj->Init();
		}
	}
	
	return false;
}

//==========================================================
// 描画に必要なオブジェクトの作成セット
//==========================================================
void CGameObjectManager::CreateBasicObject()
{
	std::shared_ptr<CGameObject> pObj = CreateGameObject();
	
	// ｶﾒﾗ
	auto cam = pObj->AddComponent<CCamera>();
	cam->GetOwner()->SetName("MainCamera");

	// ライト
	pObj = CreateGameObject();
	auto light = pObj->AddComponent<CDirectionalLight>();
	light->GetOwner()->SetName("DirectionalLight");
}

//==========================================================
// 更新配列への追加
//==========================================================
void CGameObjectManager::SetGameObject(std::shared_ptr<CGameObject> obj)
{
	if (!obj)
		return;
	
	//--- 既に存在しているなら抜ける
	for(auto & pObj : m_aGameObjList)
	{
		if (pObj == obj)
			return;
	}
	//--- 追加
	m_aGameObjList.push_back(obj);
	TagMove(obj->GetTag(), obj);
}

//==========================================================
// tag用の連想配列内の移動
//==========================================================
void CGameObjectManager::TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj)
{
	// 同一タグ
	if (obj.lock()->GetTag() == NextTag)
	{
		if (!m_aTagMap.count(NextTag))
		{
			m_aTagMap[NextTag].list.push_back(obj);
		}
		else
		{
			auto list = m_aTagMap[NextTag];
			auto it = list.FindObj(obj.lock());
			// 見つからなかったので追加
			if (it == list.list.end())
				m_aTagMap[NextTag].list.push_back(obj);
		}
		return;
	}

	//--- 現在のtagから除外
	const auto tag = obj.lock()->GetTag();
	for (auto it = m_aTagMap[tag].list.begin(); it != m_aTagMap[tag].list.end();)
	{
		if (obj.lock() == (*it).lock())
		{
			m_aTagMap[tag].list.erase(it);
			break;
		}
		++it;
	}
	
	// 変更後のtagへ移動
	m_aTagMap[NextTag].list.push_back(obj);
}

//==========================================================
// オブジェクト生成
//==========================================================
std::shared_ptr<CGameObject> CGameObjectManager::CreateGameObject(CGameObject* pObj)
{
	static int nCreateObjNum = 0;

	std::shared_ptr<CGameObject> spObj;
	//--- ｺﾋﾟｰ確認
	if (pObj)
	{
		spObj = std::make_shared<CGameObject>(*pObj);
	}
	else
	{
		spObj = std::make_shared<CGameObject>();
		// 初期名
		spObj->SetName(std::string("GameObj_" + std::to_string(++nCreateObjNum)));
	}
	
	// 自分の所属シーンを教える
	spObj.get()->SetScene(m_pAffiliationScene);

	// 自身のweakPtrを渡す
	spObj.get()->SetPtr(spObj);
	if (!pObj)
		TagMove(CTagDefault::DEFAULT, spObj);
	else
		TagMove(pObj->GetTag(), spObj);

	spObj.get()->Awake();	// 実質OnCreateな気がする
	AddGameObject(spObj);	// 追加待ちリストに追加

	return spObj;
};

//==========================================================
// オブジェクト破棄
//==========================================================
bool CGameObjectManager::DestroyObject(std::weak_ptr<CGameObject> pObj)
{
	// 検索
	auto it = std::find(m_aGameObjList.begin(), m_aGameObjList.end(), pObj.lock());
	if (it == m_aGameObjList.end())
		return false;	// 配列に居ない

	{
		// tag管理からも除外
		auto weakMap = m_aTagMap[pObj.lock()->GetTag()];
		auto mapIt = weakMap.FindObj(pObj.lock());
		if (mapIt != weakMap.list.end())
			weakMap.list.erase(mapIt);
	}

	// 非破壊検索
	if (auto itD = std::find(m_aDontDestroyList.begin(), m_aDontDestroyList.end(), pObj.lock()); itD != m_aDontDestroyList.end())
		m_aDontDestroyList.erase(itD);	// FIXME: 正しく消去されるか要確認

	// 明示的な破棄
	pObj.lock().reset();

	// 配列からの除外
	m_aGameObjList.erase(it);
	
	// 監視対象が残っている想定しないエラー
	if (pObj.lock()) 
		return false;
	
	return true;
}

//=========================================================
// オブジェクト上書き
// 非破壊オブジェクト渡し
//=========================================================
void CGameObjectManager::SetObjList(std::list<std::shared_ptr<CGameObject>> list, bool addDvive)
{
	if (addDvive)
	{
		auto work = m_aDontDestroyList;
		Uninit();
		m_aGameObjList = list;
		for (auto & obj : work)
		{
			m_aGameObjList.push_back(obj);
		}
	}
	else
	{
		Uninit();
		m_aGameObjList = list;
	}

}

//=========================================================
// オブジェクト検索(名前)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObj(std::string name)
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

//=========================================================
// オブジェクト検索(タグ名)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObjWithTag(std::string tag)
{
	if (m_aTagMap.count(tag) == 0)
	{
		//m_aTagMap[tag] = gameObjWeakList();
		return std::weak_ptr<CGameObject>();
	}
	return m_aTagMap[tag].list.begin()->lock();
}

//=========================================================
// オブジェクト検索(ﾀｸﾞｸﾗｽ)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObjWithTag(CTag tag)
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

//=========================================================
// オブジェクト検索(ﾀｸﾞｸﾗｽ)
//=========================================================
std::list<std::weak_ptr<CGameObject>> CGameObjectManager::FindGameObjctsWithTag(std::string tag)
{
	if (m_aTagMap.count(tag) == 0)
	{
		m_aTagMap[tag] = STGameObjWeakList();
		return std::list<std::weak_ptr<CGameObject>>();
	}
	return m_aTagMap[tag].list;
}