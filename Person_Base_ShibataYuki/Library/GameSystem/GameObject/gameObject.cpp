//=========================================================
// [CGameObject.cpp]
//---------------------------------------------------------
// 作成:2022/04/19 (火曜日)
// 更新:2022/05/11 タグが一致するオブジェクトを全て取得する関数作成
//			課題: 同一のコンポーネントを使う場合、mapは適切ではない
// 更新:2022/11/14 コピーコンストラクタを改造、同一のコンポーネントを生成するように変更
// 更新:2023/02/13 TODO:神ｸﾗｽ化が深刻。Factoryクラスなどを作るべき
//---------------------------------------------------------
// ゲームオブジェクト : 基底クラス
// 
//=========================================================

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Factory/componentFactory.h>

#include <CoreSystem/File/cerealize.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;


#ifdef _DEBUG
// オブジェクトの名前とtagが知りたい時に
static inline void CheckObj(CGameObject* obj)
{
	auto name = obj->GetName();
	auto tag = obj->GetTagPtr()->GetTag();
}
#endif // _DEBUG

//==========================================================
// コンストラクタ
//==========================================================
CGameObject::CGameObject()
	:m_eState(E_ObjectState::ACTIVE), m_bCameraVisible(false)
{
	if(!m_pTag)
		m_pTag = std::make_shared<CTag>();
	if(!m_pLayer)
		m_pLayer = std::make_shared<CLayer>();
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE), m_bCameraVisible(false)
{
	SetName(name);
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
}

//==========================================================
// コピーコンストラクタ
//==========================================================
CGameObject::CGameObject(const CGameObject & object)
{

#if BUILD_MODE
	this->m_objName = object.m_objName + std::to_string(++m_nCopyCnt);
#else
	this->m_objName = object.m_objName;
#endif // BUILD_MODE

	this->m_pTransform = object.m_pTransform;
	this->m_eState = object.m_eState;
	this->m_pLayer = object.m_pLayer;
	this->m_pTag = object.m_pTag;
	this->m_aComponent = object.m_aComponent;
	this->m_aComponent.clear();

	// ｺﾝﾎﾟｰﾈﾝﾄの名前から同じｺﾝﾎﾟｰﾈﾝﾄを追加
	// TODO: 各ｺﾝﾎﾟｰﾈﾝﾄの値のｺﾋﾟｰは行えない
	for (auto & component : object.m_aComponent)
	{
		// コンポーネントを保存しているｸﾗｽに追加してもらう
		if (CComponentFactory::ObjSetComponent(*this, component->GetName()))
		{
			// 最後尾に追加されているコンポーネントを取得
			auto com = this->GetComponentList().back();
			// Transform確認
			if (com->GetName() == this->m_pTransform.lock()->GetName())
				this->m_pTransform = com->BaseToDerived<CTransform>();
			//com->Init();

		}
	}
}

//==========================================================
// デストラクタ
//==========================================================
CGameObject::~CGameObject()
{
	Uninit();
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CGameObject::OnLoad()
{
	m_pTransform = GetComponent<CTransform>();

	// コンポーネントのロード呼び出し
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnLoad();
	}
}

//==========================================================
// 初期化より前に呼び出す処理
// オブジェクトやコンポーネントを追加するならここ
// 名前やタグ、レイヤーの設定もここで行う
//==========================================================
void CGameObject::Awake()
{
	// Awakeはｺﾝﾎﾟｰﾈﾝﾄ追加時に呼び出される仕様
	if (m_pTransform.lock())
		return;

	m_pTransform = AddComponent<CTransform>();
}

//==========================================================
// 初期化
// オブジェクトやコンポーネントを探すならここ
//==========================================================
void CGameObject::Init()
{
	// コンポーネント初期化呼び出し
	/*for (auto & com : m_pComponent)
	{
		com->Init();
	}*/
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->Init();
	}
}

//=========================================================
// 解放
//=========================================================
void CGameObject::Uninit()
{
	for (auto & com : m_aComponent)
		com.reset();
	m_aComponent.clear();
}

//==========================================================
// コンポーネントの更新
// この更新内でオブジェクトが破棄はされない
//==========================================================
void CGameObject::Update()
{
	for (auto & com : m_aComponent)
	{

#if _DEBUG
		auto name = com->GetName();
#endif // _DEBUG

		if (!com->IsActive())
		{
			continue;
		}

		com->Update();
	}
}

//==========================================================
// ゲームオブジェクトごとの遅い更新
//==========================================================
void CGameObject::LateUpdate()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->LateUpdate();
	}
}

//==========================================================
// 一定時間更新
//==========================================================
void CGameObject::FixedUpdate()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->FixedUpdate();
	}
}

//==========================================================
// 見えた瞬間
//==========================================================
void CGameObject::CameraTest(const bool test)
{
	// 前回写っている時
	if (m_bCameraVisible && test)
	{
		OnWillRenderObject();
	}
	// 前回写っていない、かつ、映った時
	else if(!m_bCameraVisible && test)
	{
		OnBecameVisible();
	}
	// 写っていない時
	if (!test)
	{
		OnBecameInvisible();
	}

	m_bCameraVisible = test;
}

//==========================================================
// 見えた瞬間
//==========================================================
void CGameObject::OnBecameVisible()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnBecameVisible();
	}
}

//==========================================================
// 見えなくなった瞬間
//==========================================================
void CGameObject::OnBecameInvisible()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnBecameInvisible();
	}
}

//==========================================================
// 見えている時
//==========================================================
void CGameObject::OnWillRenderObject()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnWillRenderObject();
	}
}

//==========================================================
// コンポーネント設定
//==========================================================
void CGameObject::ComponentAddPreparation(std::shared_ptr<CComponent> com)
{
	//m_pComponent.push_back(com);
	com->SetPtr(com);					// 自身のポインタの設定
	//com->SetOwner(GetPtr().lock());		// 所持gameobjectの設定
	//com->Awake();
}

//==========================================================
// コンポーネントの破棄
//==========================================================
template <class T>
bool CGameObject::RemoveComponent(std::string comName)
{
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		// 念のため確認
		if (typeid(T).name == (*it)->GetName())
		{
			m_aComponent.erase(it);
			return true;
		}
	}
	return false;
}

//==========================================================
// コンポーネント破棄
//==========================================================
bool CGameObject::RemoveComponent(std::weak_ptr<CComponent> com)
{
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		if ((*it) == com.lock())
		{
			m_aComponent.erase(it);
			return true;
		}
	}
	return false;
}

//==========================================================
// コンポーネント破棄
//==========================================================
bool CGameObject::SetComponent(std::shared_ptr<CComponent> com)
{
	// あればfalse
	for (auto & it : m_aComponent)
	{
		if (com->GetName() == it->GetName())
			return false;
	}

	com->SetOwner(GetPtr());
	 m_aComponent.push_back(com); 
	 return true;
}

//==========================================================
// 状態設定
// inline?
//==========================================================
void CGameObject::SetState(const E_ObjectState state)
{
	m_eState = state;
}

//==========================================================
// タグの移動
// ここでやるのは間違い?
//==========================================================
void CGameObject::SetObjTag(const std::string tag) 
{ 
	if(auto scene = GetScene(); scene)
		scene->GetObjManager()->TagMove(tag, GetPtr());

	m_pTag->SetTag(tag);
};

//==========================================================
// 衝突時呼び出し
//==========================================================
void CGameObject::OnCollisionEnter(CGameObject* obj)
{
#ifdef _DEBUG
	CheckObj(obj);
#endif // _DEBUG

	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionEnter(obj);
	}
}

//==========================================================
// 衝突中呼び出し
//==========================================================
void CGameObject::OnCollisionStay(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionStay(obj);
	}
}

//==========================================================
// 衝突終了時呼び出し
//==========================================================
void CGameObject::OnCollisionExit(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionExit(obj);
	}
}

//==========================================================
// 衝突時呼び出し（ﾄﾘｶﾞｰ)
//==========================================================
void CGameObject::OnTriggerEnter(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerEnter(obj);
	}
}

//==========================================================
// 衝突中呼び出し（ﾄﾘｶﾞｰ)
//==========================================================
void CGameObject::OnTriggerStay(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerStay(obj);
	}
}

//==========================================================
// 衝突終了時呼び出し（ﾄﾘｶﾞｰ)
//==========================================================
void CGameObject::OnTriggerExit(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerExit(obj);
	}
}

//--- 静的メンバ関数

//==========================================================
// 名前が一致するオブジェクト探索
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObject(std::string name)
{
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObj(name.c_str());
			if (obj.lock())
			{
				return obj;
			}
		}
	}
	return std::shared_ptr<CGameObject>();
}

//==========================================================
// タグ名が一致するオブジェクト探索
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string tag)
{
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				return obj;
			}
		}
	}
	return std::shared_ptr<CGameObject>();
}

//==========================================================
// タグ名が一致する全オブジェクト取得
//==========================================================
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(std::string tag)
{
	std::list<std::weak_ptr<CGameObject>> ret;
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto objs = scene->GetObjManager()->FindGameObjctsWithTag(tag);
			// 格納
			for (auto & obj : objs)
			{
				ret.push_back(obj);
			}
		}
	}
	return ret;
}

//==========================================================
// タグｸﾗｽが一致するオブジェクト取得
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(CTag tag)
{
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				return obj;
			}
		}
	}
	return std::shared_ptr<CGameObject>();
}

//==========================================================
// タグｸﾗｽが一致する全オブジェクト取得
//==========================================================
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(CTag tag)
{
	std::list<std::weak_ptr<CGameObject>> retList;
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				retList.push_back(obj);
			}
		}
	}
	return std::list<std::weak_ptr<CGameObject>>();
}

//==========================================================
// オブジェクト生成
// 管理クラス側に登録された後ﾎﾟｲﾝﾀを引き渡す
//==========================================================
std::weak_ptr<CGameObject> CGameObject::CopyObject(CGameObject::Ptr pObj)
{
	// シリアライズクラス作成
	CCerealize<std::shared_ptr<CGameObject>> sirial;
	{
		// ﾃﾞｰﾀを外部保存
		auto obj = pObj;
		sirial.OutputFile(obj->GetName(), COPY_DATA_GAME_OBJECT_PATH, obj);
	}

	// 一時的なオブジェクト生成
	if (auto work = std::make_shared<CGameObject>(); work)
	{
		// ﾃﾞｰﾀ読み込み
		work = sirial.InputFile(COPY_DATA_GAME_OBJECT_PATH);

		// 新しいオブジェクト生成
		auto newObj = CGameObject::CreateObject();
		// 読みこまれたコンポーネントの受け渡し
		auto comList = work->GetComponentList();
		for (auto & com : comList)
		{
			newObj.lock()->SetComponent(com);
			//--- 描画と当たり判定クラスは要請する必要があるため、Initを呼び出す
			// NOTE: 限定的なもので、正直どうなのか
			if (com->GetName().find("Renderer") != std::string::npos ||
				com->GetName().find("Collision") != std::string::npos)
			{
				com->Awake();
				com->Init();
			}
		}

		// オブジェクト破棄
		work.reset();

		return newObj;
	}

	// 失敗
	return CGameObject::Ptr();
}

//==========================================================
// オブジェクト生成
// 管理クラス側に登録された後ﾎﾟｲﾝﾀを引き渡す
//==========================================================
std::weak_ptr<CGameObject> CGameObject::CreateObject(CGameObject* pObj)
{
#ifdef _DEBUG
	auto scene = CSceneManager::Get()->GetActiveScene();
#endif // _DEBUG
	
	// ｺﾋﾟｰではない生成
	if (!pObj) 
	{
		return CSceneManager::Get()->GetActiveScene()->GetObjManager()->CreateGameObject();
	}

	return CSceneManager::Get()->GetActiveScene()->GetObjManager()->CreateGameObject(pObj);
}

//==========================================================
// 管理してほしくないオブジェクト生成
// 作るだけで管理は受け取った側に委任
//==========================================================
std::shared_ptr<CGameObject> CGameObject::CreateDebugObject(std::shared_ptr<CGameObject> pObj)
{
	if (!pObj)
		pObj = std::make_shared<CGameObject>();
	// 自身のweakPtrを渡す
	pObj.get()->SetPtr(pObj);
	pObj.get()->Awake();
	pObj.get()->Init();
	return pObj;
}

//==========================================================
// オブジェクト破棄
// manager更新時の確認で破棄される
//==========================================================
void CGameObject::Destroy(std::weak_ptr<CGameObject> pObj)
{
	// 破棄状態に変更
	pObj.lock()->SetState(E_ObjectState::DESTROY);
}

//==========================================================
// 非破壊登録
//==========================================================
void CGameObject::DontDestroy(std::weak_ptr<CGameObject> pObj)
{
	CSceneManager::Get()->GetActiveScene()->GetObjManager()->DontDestroy(pObj);
}


#ifdef BUILD_MODE

void CGameObject::ImGuiDebug()
{
	static std::vector<std::string> aStateName = {
		"ACTIVE",			// 更新状態
		"WAIT",				// 待機
		"DESTROY",			// 削除
		"TAKEOVER",			// 引き継ぎ（使いまわし)
		"STOP",				// デバッグ?
	};
	static bool isAddTag = false;
	static bool isAddLayer = false;

	//--- 状態変更
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"State"))
		{
			for (int state = 0; state < E_ObjectState::MAX_OBJECT_STATE; ++state)
			{
				if (ImGui::MenuItem(aStateName[state].c_str())) 
				{
					m_eState = (E_ObjectState)state;
					break;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	
	//--- タグ変更
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"Tag"))
		{
			auto tagList = CTag::GetNameList();
			for (int idx = 0; idx < static_cast<int>(tagList.size()); ++idx)
			{
				auto tagName = tagList[idx].c_str();
				if (ImGui::MenuItem(tagName))
				{
					GetTagPtr()->SetTag(tagName);
				}
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("Add Tag"))
			{
				isAddTag = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	CTag::ImGuiTag(isAddTag);
	
	//--- レイヤー変更
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"layer"))
		{
			auto layerList = CLayer::GetNameList(true);
			for (int idx = 0; idx < static_cast<int>(layerList.size()); ++idx)
			{
				auto tagName = layerList[idx].c_str();
				if (ImGui::MenuItem(tagName))
				{
					GetLayerPtr()->SetLayer(idx);
					break;
				}
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("Add Layer"))
			{
				isAddLayer = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	CLayer::ImGuiLayer(isAddLayer);

	ImGui::PushItemWidth(200);

	//--- 名前変更
	char name[56] = "";
	strcpy_s(name, GetName().c_str());
	Debug::SetTextAndAligned("Object Name");
	if(ImGui::InputText("##Name", name, 56))
		SetName(name);

	//--- 表示
	ImGui::Text("State:%s", aStateName[m_eState].c_str());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		if (auto select = Debug::DispComboSelect(aStateName, "State", m_eState); select != m_eState)
			m_eState = (CGameObject::E_ObjectState)select;
	}

	ImGui::Text("Tag:%s", GetTagPtr()->GetTag().c_str());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		auto tagList = CTag::GetNameList();
		if (auto select = Debug::DispComboSelect(tagList, "Tag", GetTagPtr()->GetTag().c_str()); !select.empty())
			GetTagPtr()->SetTag(select);
	}

	ImGui::Text("Layer:%d", GetLayerPtr()->GetLayer());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		auto layerList = CLayer::GetNameList();
		if (auto select = Debug::DispComboSelect(layerList, "Layer", GetLayerPtr()->GetName().c_str()); !select.empty())
			GetLayerPtr()->SetLayer(select);
	}

	ImGui::Text("Component Num:%d", GetComponentList().size());

	ImGui::PopItemWidth();

}

#endif // BUILD_MODE
