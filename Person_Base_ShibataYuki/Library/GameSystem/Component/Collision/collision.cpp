//=========================================================
//�쐬:2022/04/20 (���j��)
// �R���W�����׽(�����蔻�� ) : ���׽
// ���ƃ{�b�N�X�̓����蔻��͕�����������������?
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>

#pragma region NAME_SPACE
using namespace MySpace::Game;
using namespace MySpace::SceneManager;
#pragma endregion

//// �R���X�g���N�^
//CCollision::CCollision() 
//	:m_bIsTrigger(false), m_pTransform(nullptr), m_vOldPos(0,0,0),m_HitSubjectTag(CTag::DEFAULT),
//	m_pEnterList(0), m_pHitList(0)
//{
//}
// �����t���R���X�g���N�^
CCollision::CCollision(std::shared_ptr<CGameObject> owner, bool trigger)
	:CComponent(owner), m_bIsTrigger(trigger), m_vOldPos(0, 0, 0),
	m_pOldStayList(0),m_pHitList(0)
{
}
// �f�X�g���N�^
CCollision::~CCollision()
{
	m_pOldStayList.clear();
	m_pHitList.clear();
	m_pExitList.clear();
}
void CCollision::Awake()
{
	m_pTransform = GetOwner()->GetComponent<CTransform>();

}
// ������
void CCollision::Init()
{
	// �����蔻��̗v��
	RequestCollision();

	if(!m_pTransform.lock())
		m_pTransform = GetOwner()->GetTransform(1);
	// �ߋ����W
	m_vOldPos = m_pTransform.lock()->GetPos();
	m_vOldScale = m_pTransform.lock()->GetScale();
	if (m_vOldScale.check({ 0,0,0 }))
	{
		m_vOldScale = { 1,1,1 };
	}
}
// �X�V
void CCollision::Update()
{
	// �^�O���ύX����Ă��邩������Ȃ��̂ŁA������
	//SetColTag("");

	// �ߋ����W���X�V
	m_vOldPos = m_pTransform.lock()->GetPos();

	// �����蔻��̗v��
	//RequestCollision();
	//CSceneManager::Get().GetActiveScene()->GetObjManager()->SetColComponent(GetPtr());
}
void CCollision::RequestCollision()
{
	CSceneManager::Get().GetActiveScene()->GetObjManager()->SetColComponent(BaseToDerived<CCollision>());
}
// <Summary>
// �O������Ăяo�����
// ���������ۂ̏���
// Enter��Stay�͂����Ŕ���
// </Summary>
void CCollision::HitResponse(CCollision* other)
{
	std::weak_ptr<CGameObject> otherObj = other->GetOwner(0);
	CGameObject* pObj = otherObj.lock().get();
	
	// �g���K�[��true / �Փ˃I�u�W�F��true
	bool trigger = m_bIsTrigger | other->IsTrigger();

	// �Փ˂����I�u�W�F�N�g�̃^�O��ۑ�
	//SetColTag(pObj->GetTagPtr()->GetTag());

	// Stay
	m_pHitList.push_back(otherObj);


	// Stay:�ȑO�̃t���[���ŐڐG���Ă������m�F
	//if (auto it = std::find(m_pOldStayList.begin(), m_pOldStayList.end(), otherObj.lock()); it != m_pOldStayList.end())
	if (IsPtrIn<std::weak_ptr<CGameObject>>(m_pOldStayList, otherObj))
	{
		if (!trigger)
		{
			// ���̍��W���i�[
			//m_pTransform.lock()->SetPos(m_vOldPos);
			GetOwner()->OnCollisionStay(pObj);
		}
		else
		{
			GetOwner()->OnTriggerStay(pObj);
		}
#ifdef BUILD_MODE
		++m_nDebugStayCnt;
#endif // BUILD_MODE
	}
	else
	{	// Trigger:���̃t���[���ŐڐG�n�߂�
		if (!trigger)
		{
			// ���̍��W���i�[
			//m_pTransform.lock()->SetPos(m_vOldPos);
			GetOwner()->OnCollisionEnter(pObj);
		}
		else
		{
			GetOwner()->OnTriggerEnter(pObj);
		}
#ifdef BUILD_MODE
	++m_nDebugEnterCnt;
#endif // BUILD_MODE
	}

	// ���ꂽ�I�u�W�F�N�g�̔���
	ColObjectUpdate();
}
// <Summary>
// �G��Ă��遨 Enter �� �����ڂ��Ă��� �� stay�ۑ� �� ���ꂽ Exit
// </Summary>
void CCollision::ColObjectUpdate()
{
	// �O�t���[���̃��X�g���m�F
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_pOldStayList.begin(); it != m_pOldStayList.end(); ++it)
	{
		if (!(*it).lock()) // �폜����Ă���\�����ӂ݂�
		{
			continue;
		}

		// �i�[����Ă��Ȃ��A���ꂽ
		//if (auto res = std::find(m_pHitList.begin(), m_pHitList.end(), (*it)); res == m_pHitList.end())
		if(!IsPtrIn<std::weak_ptr<CGameObject>>(m_pHitList, (*it)))
		{
			m_pExitList.push_back((*it));
#ifdef BUILD_MODE
			++m_nDebugExitCnt;
#endif // BUILD_MODE
		}
	}

	// hit���X�g����Stay���X�g�֊i�[
	m_pOldStayList = m_pHitList;

	// ���̔���̂��߁A�N���A
	m_pHitList.clear();
}
// <Summary> 
// ���ꂽ����̂��Ƃ�������
// </Summary>
bool CCollision::ExitTell()
{
	// Enter���X�g��Stay�I�u�W�F����Ώ��O
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_pExitList.begin(); it != m_pExitList.end(); ++it)
	{
		CGameObject* pObj = (*it).lock().get();
		if (!pObj)continue;

		if (!m_bIsTrigger)
		{
			GetOwner()->OnCollisionExit(pObj);
		}
		else
		{
			GetOwner()->OnTriggerExit(pObj);
		}
	}
	m_pExitList.clear();
	return false;
}

#ifdef BUILD_MODE

void CCollision::ImGuiDebug()
{
	ImGui::Text(u8"���ݓ�����", m_pOldStayList.size());
	ImGui::Text(u8"Trigger:", m_nDebugEnterCnt);
	ImGui::Text(u8"Stay:", m_nDebugStayCnt);
	ImGui::Text(u8"Exit:", m_nDebugExitCnt);
	ImGui::Text(u8"�����蔻��p�ߋ��T�C�Y [x:%f][y:%f][z:%f]", &m_vOldScale);

	// �ď������͂����ōs��
	m_nDebugEnterCnt = m_nDebugStayCnt = m_nDebugExitCnt = 0;
}

#endif // BUILD_MODE