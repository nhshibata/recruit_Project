//========================================================
// [stateNode.cpp]
//-------------------
// �쐬:2022/11/22
//========================================================

//--- �C���N���[�h��
#include "stateNode.h"
#include "stateCondition.h"

#include "aiSystem.h"
#if _DEBUG
#endif

using namespace AI;

CAINode::EStatus CStateNode::Execute(CAISystem* system)
{
#if _DEBUG
	++system->m_nNodeCnt;
#endif // _DEBUG

	//--- �J�ڊm�F
	CAINode::Ptr next = CAINode::Ptr();
	for (auto & condition : m_pConditons)
	{	// �������������Ă���΁A��ԕύX
		next = condition->CheckTransition(system);
		if (next)
		{
			system->SetRoot(next);
			break;
		}
	}

	//--- ���g�̍X�V
	// ���L�m�[�h�̎��s���s�����A�I�[�o�[���C�h����Ă����ꍇ�͈قȂ�
	return Update(system);
}

CAINode::EStatus CStateNode::Update(CAISystem* system)
{
	//--- ���L�m�[�h�̎��s
	// �����ݒ�Ȃ�G���[
	return m_pRoot->Execute(system);
}