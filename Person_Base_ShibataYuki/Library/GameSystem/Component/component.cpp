//=========================================================
//作成:2022/04/19 (火曜日)
// コンポーネントｸﾗｽ : 基底クラス
//=========================================================

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace MySpace::Game;

CComponent::CComponent() : m_bActive(true) 
{
};
CComponent::CComponent(std::shared_ptr<CGameObject> owner) 
	:m_pOwner(owner), m_bActive(true)
{
#if BUILD_MODE
	//m_Name = GetName();
#endif // BUILD_MODE
};
CComponent::~CComponent() 
{}
//--- 基本
// *@ 生成時に呼び出される
void CComponent::Awake() 
{};
// *@ 初期化 他コンポーネントの取得などを行う
void CComponent::Init() 
{};
// *@ 終了処理
void CComponent::Uninit() 
{};
// *@ 更新 必ず実装
void CComponent::Update() 
{};
// *@ 遅い更新
void CComponent::LateUpdate() 
{};
// *@ 一定時間の更新
void CComponent::FixedUpdate() 
{}
// *@ 読み込み時呼び出し関数
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
// *@ 接し続けているオブジェクトが渡される
void CComponent::OnCollisionStay(CGameObject* obj) 
{};
// *@ 接し始めたオブジェクトが渡される
void CComponent::OnCollisionEnter(CGameObject* obj) 
{};
// *@ 離れたオブジェクトが渡される
void CComponent::OnCollisionExit(CGameObject* obj) 
{};
// *@ 接し続けているオブジェクトが渡される(トリガー)
void CComponent::OnTriggerStay(CGameObject* obj) 
{};
// *@ 接し始めたオブジェクトが渡される(トリガー)
void CComponent::OnTriggerEnter(CGameObject* obj) 
{};
// *@ 離れたオブジェクトが渡される(トリガー)
void CComponent::OnTriggerExit(CGameObject* obj) 
{};
// 持ち主がActiveでなければActiveとは言えない
bool CComponent::IsActive() 
{
	return m_bActive && m_pOwner.lock()->IsActive();
}
CTransform* CComponent::Transform() const
{ 
	if (!m_pOwner.lock())
		return nullptr;

	return m_pOwner.lock()->GetTransform();
}
std::string CComponent::Tag()const 
{
	return m_pOwner.lock()->GetTagPtr()->GetTag(); 
}
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