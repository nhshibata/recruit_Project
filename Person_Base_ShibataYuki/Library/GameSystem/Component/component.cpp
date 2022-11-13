//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �R���|�[�l���g�׽ : ���N���X
//=========================================================

//--- �C���N���[�h��
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
// �����傪Active�łȂ����Active�Ƃ͌����Ȃ�
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