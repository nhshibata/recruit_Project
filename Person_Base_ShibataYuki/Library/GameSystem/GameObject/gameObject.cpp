//=========================================================
// [CGameObject.cpp]
//---------------------------------------------------------
// 作成:2022/04/19 (火曜日)
// 更新:2022/05/11 タグが一致するオブジェクトを全て取得する関数作成
//			課題: 同一のコンポーネントを使う場合、mapは適切ではない
// 更新:2022/11/14 コピーコンストラクタを改造、同一のコンポーネントを生成するように変更
//---------------------------------------------------------
// ゲームオブジェクト : 基底クラス
// 
//=========================================================

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

#include <ImGui/imgui.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Factory/componentFactory.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;

//--- 静的メンバ変数
//CGameObject::GAMEOBJECT CGameObject::m_pObjects;

#ifdef _DEBUG
// オブジェクトの名前とtagが知りたい時に
static inline void CheckObj(CGameObject* obj)
{
	auto name = obj->GetName();
	auto tag = obj->GetTagPtr()->GetTag();
}
#endif // _DEBUG

// コンストラクタ
CGameObject::CGameObject()
	:m_eState(E_ObjectState::ACTIVE)
{
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
}
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE)
{
	SetName(name);
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
}
// コピーコンストラクタ
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
	//this->m_pComponent = object.m_pComponent;
	// ｺﾝﾎﾟｰﾈﾝﾄの名前から同じｺﾝﾎﾟｰﾈﾝﾄを追加
	// TODO: 各ｺﾝﾎﾟｰﾈﾝﾄの値のｺﾋﾟｰは行えない
	for (auto & component : object.m_pComponent)
	{
		// コンポーネントを保存しているｸﾗｽに追加してもらう
		if (CComponentFactory::ObjSetComponent(*this, component->GetName()))
		{
			// 最後尾に追加されているコンポーネントを取得
			//auto com = this->GetComponentList().back();
			//com->Init();
		}
	}
}
CGameObject::~CGameObject()
{
	for (auto & com : m_pComponent)
		com.reset();
	m_pComponent.clear();
}
void CGameObject::OnLoad()
{
	m_pTransform = GetComponent<CTransform>();

	// コンポーネントのロード呼び出し
	for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		(*it)->OnLoad();
	}
}
// *初期化より前に呼び出す処理
// *オブジェクトやコンポーネントを追加するならここ
// *名前やタグ、レイヤーの設定もここで行う
void CGameObject::Awake()
{
	// Awakeはｺﾝﾎﾟｰﾈﾝﾄ追加時に呼び出される仕様
	if (m_pTransform.lock())
		return;

	m_pTransform = AddComponent<CTransform>();
}
// *初期化
// *オブジェクトやコンポーネントを探すならここ
void CGameObject::Init()
{
	// コンポーネントの初期化
	for (auto & com : m_pComponent)
	{
		com->Init();
	}
}
// コンポーネントの更新
// この更新内でオブジェクトが破棄はされない
void CGameObject::Update()
{
	for (auto & com : m_pComponent)
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
// ゲームオブジェクトごとの遅い更新
void CGameObject::LateUpdate()
{
	for (auto & com : m_pComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->LateUpdate();
	}
}
// 一定時間の更新
void CGameObject::FixedUpdate()
{
	for (auto & com : m_pComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->FixedUpdate();
	}
}
// コンポーネントの追加
std::shared_ptr<CComponent> CGameObject::AddComponent(std::shared_ptr<CComponent> com)
{
	m_pComponent.push_back(com);
	com->SetOwner(GetPtr()); 

	return com; 
}
// コンポーネントの破棄
template <class T>
bool CGameObject::RemoveComponent(std::string comName)
{
	COMPONENT::iterator it;
	for (it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		if (typeid(T).name == (*it)->GetName())
		{
			m_pComponent.erase(it);
			return true;
		}
	}
	return false;
}
// コンポーネント破棄
bool CGameObject::RemoveComponent(std::weak_ptr<CComponent> com)
{
	COMPONENT::iterator it;
	for (it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		if ((*it) == com.lock())
		{
			m_pComponent.erase(it);
			return true;
		}
	}
	return false;
}
// inline?
void CGameObject::SetState(const E_ObjectState state)
{
	m_eState = state;
}
// タグの移動
// ここでやるのは間違い?
void CGameObject::SetTag(const std::string tag) 
{ 
	if(auto scene = GetScene(); scene)
		scene->GetObjManager()->TagMove(tag, GetPtr());

	m_pTag->SetTag(tag);
};
// 衝突
void CGameObject::OnCollisionEnter(CGameObject* obj)
{
#ifdef _DEBUG
	CheckObj(obj);
#endif // _DEBUG

	for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionEnter(obj);
	}
}
void CGameObject::OnCollisionStay(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionStay(obj);
	}
}
void CGameObject::OnCollisionExit(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionExit(obj);
	}
}
void CGameObject::OnTriggerEnter(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerEnter(obj);
	}
}
void CGameObject::OnTriggerStay(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerStay(obj);
	}
}
void CGameObject::OnTriggerExit(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerExit(obj);
	}
}
//--- 静的メンバ関数
std::weak_ptr<CGameObject> CGameObject::FindGameObject(std::string name)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene()) 
	{
		auto obj = scene->GetObjManager()->FindGameObj(name.c_str());
		if (obj.lock()) 
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string tag)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(std::string tag)
{
	std::list<std::weak_ptr<CGameObject>> ret;
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto objs = scene->GetObjManager()->FindGameObjctsWithTag(tag);
		// 格納
		for (auto & obj : objs)
		{
			ret.push_back(obj);
		}
	}
	return ret;
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(CTag tag)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(CTag tag)
{
	std::list<std::weak_ptr<CGameObject>> retList;
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			retList.push_back(obj);
		}
	}
	return std::list<std::weak_ptr<CGameObject>>();
}
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
// 作るだけで管理は受け取った側に委任
std::shared_ptr<CGameObject> CGameObject::CreateDebugObject(std::shared_ptr<CGameObject> pObj)
{
	// 自身のweakPtrを渡す
	pObj.get()->SetPtr(pObj);

	// 初期名
	pObj.get()->Awake();	// 実質OnCreateな気がする
	pObj.get()->Init();
	return pObj;
}
void CGameObject::Destroy(std::weak_ptr<CGameObject> pObj)
{
	// 破棄状態に変更
	// manager更新時の確認で破棄される
	pObj.lock()->SetState(E_ObjectState::DESTROY);
	/*for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		if (scene->GetObjManager()->DestroyObject(pObj))
		{
			return;
		}
	}*/
}
void CGameObject::DontDestroy(std::weak_ptr<CGameObject> pObj)
{
	CSceneManager::Get()->GetActiveScene()->GetObjManager()->DontDestroy(pObj);
}

#ifdef BUILD_MODE

void CGameObject::ImGuiDebug()
{
	static const char* szState[CGameObject::MAX_OBJECT_STATE] = {
		"ACTIVE",				// 更新状態
		"WAIT",				// 待機
		"DESTROY",			// 削除
		"TAKEOVER",			// 引き継ぎ（使いまわし)
		"STOP",				// デバッグ?
	};

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"State"))
		{
			for (int state = 0; state < E_ObjectState::MAX_OBJECT_STATE; ++state)
			{
				if (ImGui::MenuItem(szState[state])) {
					m_eState = (E_ObjectState)state;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"tag"))
		{
			auto tagList = CTagManager::Get()->GetNameList();
			for (int state = 0; state < static_cast<int>(tagList.size()); ++state)
			{
				auto tagName = tagList[state].c_str();
				if (ImGui::MenuItem(tagName)) {
					GetTagPtr()->SetTag(tagName);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	char name[256] = "";
	strcpy_s(name, GetName().c_str());
	ImGui::InputText(u8"*名前:", name, 256);
	SetName(name);
	ImGui::SameLine();
	ImGui::Text(u8"*Tag:%s", GetTagPtr()->GetTag().c_str());
	ImGui::SameLine();
	if (ImGui::BeginCombo(u8"*Layer:%d", "1"))
	{
		// TODO: 途中
		for (int cnt = 0; cnt < 5; cnt++)
		{
			auto label = std::to_string(cnt).c_str(); 
			
			if (bool bg = ImGui::Selectable(label, &bg) ; bg)
			{
				GetLayerPtr()->SetLayer(cnt);
			}
		}
		
		ImGui::EndCombo();
	}
	ImGui::Text(u8"*ｺﾝﾎﾟｰﾈﾝﾄ数:%d", GetComponentList().size());
	ImGui::SameLine();
	ImGui::Text(u8"*state:%s", szState[m_eState]);
	//ImGui::RadioButton("state", &m_eState, E_ObjectState::ACTIVE);
}

#endif // BUILD_MODE
