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

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::SceneManager;

//==========================================================
// �R���X�g���N�^
//==========================================================
CCollision::CCollision()
	:m_nSystemIdx(-1)
{
}

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CCollision::CCollision(std::shared_ptr<CGameObject> owner, bool trigger)
	:CComponent(owner), m_bIsTrigger(trigger), m_vOldPos(0, 0, 0),
	m_aOldHitList(0),m_aHitList(0), m_nSystemIdx(-1)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCollision::~CCollision()
{
	//--- �o�^����
	if(m_nSystemIdx != -1)
		CSceneManager::Get()->GetCollisionSystem()->ExecutSystem(m_nSystemIdx);

	m_aOldHitList.clear();
	m_aHitList.clear();
	m_aExitList.clear();
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CCollision::OnLoad()
{
	RequestCollision();
}

//==========================================================
// �������Ăяo��
//==========================================================
void CCollision::Awake()
{
	// �����蔻��̗v��
	RequestCollision();
}

//==========================================================
// ������
//==========================================================
void CCollision::Init()
{

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
	if(m_nSystemIdx == -1)
		m_nSystemIdx = CSceneManager::Get()->GetCollisionSystem()->RegistToSystem(BaseToDerived<CCollision>());
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
	
	if (this == other)
		return;

	// �g���K�[��true / �Փ˃I�u�W�F��true
	bool trigger = m_bIsTrigger | other->IsTrigger();

	// Enter
	m_aHitList.push_back(otherObj);

	// Stay:�ȑO�̃t���[���ŐڐG���Ă������m�F
	bool found = false;
	for (auto & obj : m_aOldHitList)
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
			GetOwner()->OnCollisionStay(pObj);
			pObj->OnCollisionStay(GetOwner());
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
			GetOwner()->OnCollisionEnter(pObj);
			pObj->OnCollisionEnter(GetOwner());
		}
		else
		{
			GetOwner()->OnTriggerEnter(pObj);
		}

#ifdef BUILD_MODE
	++m_nDebugEnterCnt;
#endif // BUILD_MODE

	}

	
}

//==========================================================
// �Փˏ�Ԋm�F
// �G��Ă��遨 Enter �� �����ڂ��Ă��� �� stay�ۑ� �� ���ꂽ Exit
//==========================================================
void CCollision::ColObjectUpdate()
{
	//--- �O�t���[���̃��X�g���m�F
	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_aOldHitList.begin(); it != m_aOldHitList.end(); ++it)
	{
		if (!(*it).lock()) // �폜����Ă���\�����ӂ݂�
			continue;

		// �i�[����Ă��Ȃ��A���ꂽ		
		bool res = false;
		for (auto & obj : m_aHitList)
		{	
			if (obj.lock() == (*it).lock())
			{	// �i�[����Ă���
				res = true;
				break;
			}
		}
		if (res)
			continue;

		m_aExitList.push_back((*it));

#ifdef BUILD_MODE
		++m_nDebugExitCnt;
#endif // BUILD_MODE

	}


}

//==========================================================
// �ՓˏI���m�F
// ���ꂽ����̂��Ƃ�������
//==========================================================
bool CCollision::ExitTell()
{
	// ���ꂽ�I�u�W�F�N�g�̔���
	ColObjectUpdate();

	for (std::list<std::weak_ptr<CGameObject>>::iterator it = m_aExitList.begin(); it != m_aExitList.end(); ++it)
	{
		CGameObject* pObj = (*it).lock().get();
		if (!pObj)
			continue;

		if (!m_bIsTrigger)
		{
			GetOwner()->OnCollisionExit(pObj);
		}
		else
		{
			GetOwner()->OnTriggerExit(pObj);
		}
	}

	// hit���X�g����Stay���X�g�֊i�[
	m_aOldHitList.clear();
	m_aOldHitList = m_aHitList;
	// ���̔���̂��߁A�N���A
	m_aHitList.clear();
	m_aExitList.clear();
	
	return false;
}


#ifdef BUILD_MODE

void CCollision::ImGuiDebug()
{

	/*Debug::SetTextAndAligned("current hit");
	ImGui::Text("%d", m_aOldHitList.size());*/

	Debug::SetTextAndAligned("Enter");
	ImGui::Text("%d", m_nDebugEnterCnt);

	Debug::SetTextAndAligned("Stay");
	ImGui::Text("%d", m_nDebugStayCnt);

	Debug::SetTextAndAligned("Exit");
	ImGui::Text("%d", m_nDebugExitCnt);

	/*Debug::SetTextAndAligned(u8"�����蔻��p�ߋ��T�C�Y");
	ImGui::Text("[x:%.5f][y:%.5f][z:%.5f]", &m_vOldScale);*/

	Debug::SetTextAndAligned("Is Trigger");
	ImGui::Checkbox("##Is Trigger", (bool*)&m_bIsTrigger);
	
	Debug::SetTextAndAligned("Center");
	ImGui::DragFloat3("##Center", (float*)m_vCenter);

	// �ď������͂����ōs��
	/*m_nDebugEnterCnt = 0;*/
	m_nDebugStayCnt = 0;
	m_nDebugExitCnt = 0;

}

#endif // BUILD_MODE