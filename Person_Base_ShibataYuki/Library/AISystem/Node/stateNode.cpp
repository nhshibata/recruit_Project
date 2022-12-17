//========================================================
// [stateNode.cpp]
//-------------------
// �쐬:2022/11/22
//========================================================

//--- �C���N���[�h��
#include <AISystem/Node/stateNode.h>
#include <AISystem/stateCondition.h>
#include <AISystem/System/aiSystem.h>

using namespace AI;

CAINode::EStatus CStateNode::Execute(CAISystem* system)
{
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