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
#if BUILD_MODE
	//m_Name = GetName();
#endif // BUILD_MODE
};
CComponent::~CComponent() 
{}
//--- ��{
// *@ �������ɌĂяo�����
void CComponent::Awake() 
{};
// *@ ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
void CComponent::Init() 
{};
// *@ �I������
void CComponent::Uninit() 
{};
// *@ �X�V �K������
void CComponent::Update() 
{};
// *@ �x���X�V
void CComponent::LateUpdate() 
{};
// *@ ��莞�Ԃ̍X�V
void CComponent::FixedUpdate() 
{}
// *@ �ǂݍ��ݎ��Ăяo���֐�
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
// *@ �ڂ������Ă���I�u�W�F�N�g���n�����
void CComponent::OnCollisionStay(CGameObject* obj) 
{};
// *@ �ڂ��n�߂��I�u�W�F�N�g���n�����
void CComponent::OnCollisionEnter(CGameObject* obj) 
{};
// *@ ���ꂽ�I�u�W�F�N�g���n�����
void CComponent::OnCollisionExit(CGameObject* obj) 
{};
// *@ �ڂ������Ă���I�u�W�F�N�g���n�����(�g���K�[)
void CComponent::OnTriggerStay(CGameObject* obj) 
{};
// *@ �ڂ��n�߂��I�u�W�F�N�g���n�����(�g���K�[)
void CComponent::OnTriggerEnter(CGameObject* obj) 
{};
// *@ ���ꂽ�I�u�W�F�N�g���n�����(�g���K�[)
void CComponent::OnTriggerExit(CGameObject* obj) 
{};
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