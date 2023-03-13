//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �R���|�[�l���g�׽ : ���N���X
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CComponent::CComponent() 
	: m_bActive(true)
{
};

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CComponent::CComponent(std::shared_ptr<CGameObject> owner) 
	:m_pOwner(owner), m_bActive(true)
{
#if BUILD_MODE
	//m_Name = GetName();
#endif // BUILD_MODE

};

//==========================================================
// ��߰�R���X�g���N�^
//==========================================================
CComponent::CComponent(const CComponent& copy)
{
	this->m_pOwner = copy.m_pOwner;
	this->m_bActive = copy.m_bActive;
	this->m_pSelfPtr = copy.m_pSelfPtr;
};

//==========================================================
// �f�X�g���N�^
//==========================================================
CComponent::~CComponent() 
{

}

//==========================================================
//--- ���֐�
//==========================================================

//==========================================================
// �������ɌĂяo�����
//==========================================================
//#pragma optimize("", off)
//void CComponent::Awake() 
//{
//}
//#pragma optimize("", on)

//==========================================================
// ������ 
// ���R���|�[�l���g�̎擾�Ȃǂ��s��
//==========================================================
#pragma optimize("", off)
void CComponent::Init() 
{
}
#pragma optimize("", on)

//==========================================================
// �I������
//==========================================================
void CComponent::Uninit() 
{
}

//==========================================================
// �X�V
//==========================================================
void CComponent::Update() 
{
}

//==========================================================
// �x���X�V
//==========================================================
void CComponent::LateUpdate() 
{
}

//==========================================================
// ��莞�Ԃ̍X�V
//==========================================================
void CComponent::FixedUpdate() 
{
}

//==========================================================
// �ǂݍ��ݎ��Ăяo���֐�
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
// �������u��
//==========================================================
void CComponent::OnBecameVisible()
{

}

//==========================================================
// �����Ȃ��Ȃ����u��
//==========================================================
void CComponent::OnBecameInvisible()
{

}

//==========================================================
// �����Ă��鎞
//==========================================================
void CComponent::OnWillRenderObject()
{

}

//==========================================================
// �Փˎ��Ăяo��
// �ڂ������Ă���I�u�W�F�N�g���n�����
//==========================================================
void CComponent::OnCollisionEnter(CGameObject* obj)
{
}

//==========================================================
// �Փˏ�ԌĂяo��
// �ڂ��n�߂��I�u�W�F�N�g���n�����
//==========================================================
void CComponent::OnCollisionStay(CGameObject* obj) 
{
}

//==========================================================
// �ՓˏI���Ăяo��
// ���ꂽ�I�u�W�F�N�g���n�����
//==========================================================
void CComponent::OnCollisionExit(CGameObject* obj) 
{
}

//==========================================================
// �Փˎ��Ăяo��
// �ڂ��n�߂��I�u�W�F�N�g���n�����(�g���K�[)
//==========================================================
void CComponent::OnTriggerEnter(CGameObject* obj) 
{
}

//==========================================================
// �Փˏ�Ԏ��Ăяo��
// �ڂ������Ă���I�u�W�F�N�g���n�����(�g���K�[)
//==========================================================
void CComponent::OnTriggerStay(CGameObject* obj) 
{
}

//==========================================================
// �ՓˏI�����Ăяo��
// ���ꂽ�I�u�W�F�N�g���n�����(�g���K�[)
//==========================================================
void CComponent::OnTriggerExit(CGameObject* obj) 
{
}

//==========================================================
// ��è�ފm�F
// �����傪Active�łȂ����Active�Ƃ͌����Ȃ�
//==========================================================
bool CComponent::IsActive() 
{
	return m_bActive && m_pOwner.lock()->IsActive();
}

//==========================================================
// Transform�擾
//==========================================================
CTransform* CComponent::Transform() const
{ 
	if (!m_pOwner.lock())
		return nullptr;

	return m_pOwner.lock()->GetTransform();
}

//==========================================================
// tag�擾
//==========================================================
std::string CComponent::Tag()const 
{
	return m_pOwner.lock()->GetTagPtr()->GetTag(); 
}

//==========================================================
// Layer�擾
//==========================================================
int CComponent::GetLayer() const
{
	return m_pOwner.lock()->GetLayerPtr()->GetLayer(); 
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