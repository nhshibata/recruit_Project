//=========================================================
//作成:2022/04/19 (火曜日)
// コンポーネントｸﾗｽ : 基底クラス
//=========================================================

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CComponent::CComponent() : m_bActive(true) 
{
};

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CComponent::CComponent(std::shared_ptr<CGameObject> owner) 
	:m_pOwner(owner), m_bActive(true)
{
#if BUILD_MODE
	//m_Name = GetName();
#endif // BUILD_MODE

};

//==========================================================
// ｺﾋﾟｰコンストラクタ
//==========================================================
CComponent::CComponent(const CComponent& copy)
{
	this->m_pOwner = copy.m_pOwner;
	this->m_bActive = copy.m_bActive;
	this->m_pSelfPtr = copy.m_pSelfPtr;
};

//==========================================================
// デストラクタ
//==========================================================
CComponent::~CComponent() 
{

}

//==========================================================
//--- 基底関数
//==========================================================

//==========================================================
// 生成時に呼び出される
//==========================================================
//#pragma optimize("", off)
//void CComponent::Awake() 
//{
//}
//#pragma optimize("", on)

//==========================================================
// 初期化 
// 他コンポーネントの取得などを行う
//==========================================================
#pragma optimize("", off)
void CComponent::Init() 
{
}
#pragma optimize("", on)

//==========================================================
// 終了処理
//==========================================================
void CComponent::Uninit() 
{
}

//==========================================================
// 更新
//==========================================================
void CComponent::Update() 
{
}

//==========================================================
// 遅い更新
//==========================================================
void CComponent::LateUpdate() 
{
}

//==========================================================
// 一定時間の更新
//==========================================================
void CComponent::FixedUpdate() 
{
}

//==========================================================
// 読み込み時呼び出し関数
//==========================================================
void CComponent::OnLoad()
{
#ifdef _DEBUG
	/*auto owner = m_pOwner.lock();
	if (m_pSelfPtr.lock())
	{
		[[maybe_unused]] auto name = m_pSelfPtr.lock()->GetName();
	}*/
#endif // _DEBUG
}

//==========================================================
// 衝突時呼び出し
// 接し続けているオブジェクトが渡される
//==========================================================
void CComponent::OnCollisionEnter(CGameObject* obj) 
{
}

//==========================================================
// 衝突状態呼び出し
// 接し始めたオブジェクトが渡される
//==========================================================
void CComponent::OnCollisionStay(CGameObject* obj) 
{
}

//==========================================================
// 衝突終了呼び出し
// 離れたオブジェクトが渡される
//==========================================================
void CComponent::OnCollisionExit(CGameObject* obj) 
{
}

//==========================================================
// 衝突時呼び出し
// 接し始めたオブジェクトが渡される(トリガー)
//==========================================================
void CComponent::OnTriggerEnter(CGameObject* obj) 
{
}

//==========================================================
// 衝突状態時呼び出し
// 接し続けているオブジェクトが渡される(トリガー)
//==========================================================
void CComponent::OnTriggerStay(CGameObject* obj) 
{
}

//==========================================================
// 衝突終了時呼び出し
// 離れたオブジェクトが渡される(トリガー)
//==========================================================
void CComponent::OnTriggerExit(CGameObject* obj) 
{
}

//==========================================================
// ｱｸﾃｨﾌﾞ確認
// 持ち主がActiveでなければActiveとは言えない
//==========================================================
bool CComponent::IsActive() 
{
	return m_bActive && m_pOwner.lock()->IsActive();
}

//==========================================================
// Transform取得
//==========================================================
CTransform* CComponent::Transform() const
{ 
	if (!m_pOwner.lock())
		return nullptr;

	return m_pOwner.lock()->GetTransform();
}

//==========================================================
// tag取得
//==========================================================
std::string CComponent::Tag()const 
{
	return m_pOwner.lock()->GetTagPtr()->GetTag(); 
}

//==========================================================
// Layer取得
//==========================================================
int CComponent::GetLayer() const
{
	return *m_pOwner.lock()->GetLayerPtr()->GetLayer(); 
}


//template <class T>
//std::shared_ptr<T> CComponent::AddComponent()
//{
//	return m_pOwner.lock()->AddComponent<T>();
//}
//template <class T>
//T* CComponent::AddComponent(int n)
//{
//	return m_pOwner.lock()->AddComponent<T>().get();
//}
//void CComponent::AddComponent(std::shared_ptr<CComponent> com) 
//{
//	m_pOwner.lock()->AddComponent(com);
//}
////==========================================================
//template std::shared_ptr<CComponent> CComponent::AddComponent<CComponent>();
//template CComponent* CComponent::AddComponent<CComponent>(int n);
////==========================================================
//
//template <class T>
//T* CComponent::GetComponent()
//{
//	std::weak_ptr<T> com = m_pOwner.lock()->GetComponent<T>();
//	return com.lock().get();
//};
//==========================================================
//template CComponent* CComponent::GetComponent<CComponent>();
//==========================================================