//=========================================================
//�쐬:2022/04/20 (���j��)
// �R���W�����׽(�����蔻�� ) : ���׽
// ���ƃ{�b�N�X�̓����蔻��͕�����������������?
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::SceneManager;

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CCollision::CCollision(std::shared_ptr<CGameObject> owner, bool trigger)
	:CComponent(owner), m_bIsTrigger(trigger), m_vOldPos(0, 0, 0),
	m_pOldStayList(0),m_pHitList(0), m_nSystemIdx(-1)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCollision::~CCollision()
{
	CSceneManager::Get().GetCollisionSystem()->ExecutSystem(m_nSystemIdx);

	m_pOldStayList.clear();
	m_pHitList.clear();
	m_pExitList.clear();
}

//==========================================================
// �������Ăяo��
//==========================================================
void CCollision::Awake()
{
}

//==========================================================
// ������
//==========================================================
void CCollision::Init()
{
	// �����蔻��̗v��
	RequestCollision();

	// �ߋ����W
	m_vOldPos = Transform()->GetPos();
	m_vOldScale = Transform()->GetScale();
	if (m_vOldScale.check({ 0,0,0 }))
	{
		m_vOldScale = { 1,1,1 };
	}
}

//==========================================================
// �X�V
//==========================================================
void CCollision::Update()
{
	// �ߋ����W���X�V
	m_vOldPos = Transform()->GetPos();

	// �����蔻��̗v��
	//RequestCollision();
}

//==========================================================
// �V�X�e���ւ̐\��
//==========================================================
void CCollision::RequestCollision()
{
	m_nSystemIdx = CSceneManager::Get().GetCollisionSystem()->RegistToSystem(BaseToDerived<CCollision>());
}

//==========================================================
// �Փˎ��֐��Ăяo��
// �O������Ăяo�����
// ���������ۂ̏���
// Enter��Stay�͂����Ŕ���
//==========================================================
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
	bool found = false;
	for (auto & obj : m_pOldStayList)
	{
		if (obj.lock() == otherObj.lock())
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		if (!trigger)
		{
			// ���̍��W���i�[
			//Transform()->SetPos(m_vOldPos);
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
			//Transform()->SetPos(m_vOldPos);
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

//==========================================================
// �Փˏ�Ԋm�F
// �G��Ă��遨 Enter �� �����ڂ��Ă��� �� stay�ۑ� �� ���ꂽ Exit
//==========================================================
void CCollision::ColObjectUpdate()
{
	//--- �O�t���[���̃��X�g���m�F
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_pOldStayList.begin(); it != m_pOldStayList.end(); ++it)
	{
		if (!(*it).lock()) // �폜����Ă���\�����ӂ݂�
			continue;

		// �i�[����Ă��Ȃ��A���ꂽ
		//if (auto found = std::find(m_pHitList.begin(), m_pHitList.end(), (*it)); found == m_pHitList.end())
		bool res = false;
		for (auto & obj : m_pHitList)
		{	
			if ((*it).lock() == (*it).lock())
			{	// �i�[����Ă���
				res = true;
				break;
			}
		}
		if (res)
			continue;

		m_pExitList.push_back((*it));
#ifdef BUILD_MODE
		++m_nDebugExitCnt;
#endif // BUILD_MODE

	}

	// hit���X�g����Stay���X�g�֊i�[
	m_pOldStayList = m_pHitList;

	// ���̔���̂��߁A�N���A
	m_pHitList.clear();
}

//==========================================================
// �ՓˏI���m�F
// ���ꂽ����̂��Ƃ�������
//==========================================================
bool CCollision::ExitTell()
{
	//--- Enter���X�g��Stay�I�u�W�F������Ώ��O
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
	ImGui::Text(u8"Enter:", m_nDebugEnterCnt);
	ImGui::Text(u8"Stay:", m_nDebugStayCnt);
	ImGui::Text(u8"Exit:", m_nDebugExitCnt);
	ImGui::Text(u8"�����蔻��p�ߋ��T�C�Y [x:%f][y:%f][z:%f]", &m_vOldScale);
	ImGui::Checkbox(u8"Trigger", (bool*)&m_bIsTrigger);
	ImGui::DragFloat3(u8"Center", (float*)m_vCenter);

	// �ď������͂����ōs��
	m_nDebugEnterCnt = m_nDebugStayCnt = m_nDebugExitCnt = 0;

}

#endif // BUILD_MODE