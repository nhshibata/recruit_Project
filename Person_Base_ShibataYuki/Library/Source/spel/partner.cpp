//=========================================================
// [partner.h]
// �쐬:2022/12/15
//---------------------------------------------------------
// �p�[�g�i�[���׽
//=========================================================

//--- �C���N���[�h��
#include <Source/spel/partner.h>
#include <Source/spel/partnerNode.h>
#include <AISystem/System/aiSystem.h>
#include <AISystem/nodeUtil.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace Spell;

//=========================================================
CPartnerSupport::CPartnerSupport()
{
	for (int i = 0; i < 9; ++i)
		m_aPos[i] = -1;
}

CPartnerSupport::~CPartnerSupport()
{

}

const int CPartnerSupport::Bind(int idx)
{
	if (idx != -1)
		return idx;

	for (int i = 0; i < 9; i++)
	{
		if (m_aPos[i] != 1)
		{
			m_aPos[i] = 1;
			return i;
		}
	}
	return -1;
}

void CPartnerSupport::Release(const int idx)
{
	if (idx == -1)
		return;
	m_aPos[idx] = -1;
}

int CPartnerSupport::GetPos(int idx, Vector3& set)
{
	if (idx == -1)
		return -1;
	
	const float DISTANCE = 30.0f;
	Vector3 ret = m_pTarget.lock()->GetTransform()->GetPos();

	switch (idx)
	{
		case 0:	// ���΂ߏ�
			ret.x += m_pTarget.lock()->GetTransform()->right().x * -1 * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * DISTANCE;
			break;
		case 1: // ��
			ret.x += m_pTarget.lock()->GetTransform()->forward().x * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * DISTANCE;
			break;
		case 2: // �E�΂ߏ�
			ret.x += m_pTarget.lock()->GetTransform()->right().x * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * DISTANCE;
			break;
		case 3:	// ��
			ret.x += m_pTarget.lock()->GetTransform()->right().x * -1 * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->right().z * -1 * DISTANCE;
			break;
		case 5: // �E
			ret.x += m_pTarget.lock()->GetTransform()->right().x * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->right().z * DISTANCE;
			break;
		case 4:
			m_aPos[idx] = -1;
			return -1;
		case 6: // ���΂߉�
			ret.x += m_pTarget.lock()->GetTransform()->right().x * -1 * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * -1 * DISTANCE;
			break;
		case 7:
			ret.x += m_pTarget.lock()->GetTransform()->forward().x * -1 * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * -1 * DISTANCE;
			break;
		case 8:
			ret.x += m_pTarget.lock()->GetTransform()->right().x * DISTANCE;
			ret.z += m_pTarget.lock()->GetTransform()->forward().z * -1 * DISTANCE;
			break;
		default:
			break;
	}
	set = ret;
	
	return idx;
}

//=========================================================
//=========================================================
CPartner::CPartner(CGameObject::Ptr owner)
	:CComponent(owner)
{
	//--- system�ǉ�
	m_pAiSystem = AddComponent<AI::CAISystem>();
	//m_pAiSystem.lock()->SetData(&m_Param);

	// selector
	auto select1 = m_pAiSystem.lock()->MakeNode<AI::CSelectorNode>();
	auto seqence1 = m_pAiSystem.lock()->MakeNode<AI::CSequencerNode>();
	// ���ݒ�
	m_pAiSystem.lock()->SetRoot(select1);
	// 1.sequence
	select1->AddNode(seqence1);
	auto move = m_pAiSystem.lock()->MakeNode<Spell::CIsMoveNode>();
	auto stop = m_pAiSystem.lock()->MakeNode<Spell::CStopNode>();
	seqence1->AddNode(move);	// �ړ�
	seqence1->AddNode(stop);	// �ړ����I���ΐݒ莞�Ԓ�~
	
	

}

void CPartner::Awake()
{

}

void CPartner::Update()
{

}

