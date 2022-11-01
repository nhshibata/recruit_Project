//=========================================================
//作成:2022/04/19 (火曜日)
// コンポーネントｸﾗｽ : 基底クラス
//=========================================================


#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace MySpace::Game;

CComponent::CComponent() : m_bActive(true) 
{
};
CComponent::CComponent(std::shared_ptr<CGameObject> owner) :m_pOwner(owner), m_bActive(true)
{
};
void CComponent::OnLoad()
{
#ifdef _DEBUG

	auto owner = m_pOwner.lock();
	if (m_spThisPtr.lock())
	{
		[[maybe_unused]] auto name = m_spThisPtr.lock()->GetName();
	}
#endif // _DEBUG
}
CTransform* CComponent::Transform() 
{ 
	WEAK_LOCK(m_pOwner); 
	return GetOwner()->GetTransform();
}
void CComponent::AddComponent(std::shared_ptr<CComponent> com) 
{
	GetOwner()->AddComponent(com); 
}
std::string CComponent::Tag()const 
{
	return m_pOwner.lock()->GetTagPtr()->GetTag(); 
}