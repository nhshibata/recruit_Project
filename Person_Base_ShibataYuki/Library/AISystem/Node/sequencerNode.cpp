//========================================================
// [SequencerNode.cpp]
//------------------
// �쐬:2022/12/02
//------------------
// �q�m�[�h��J�ڂ���
// �r���Ŏ��s���A��Β��f
//========================================================

//--- �C���N���[�h��
#include "sequencerNode.h"

using namespace AI;

CAINode::EStatus CSequencerNode::Execute(CAISystem* sys)
{
	//--- ���݃m�[�h�̎��s
	// ���q�m�[�h��ParentNode�Ȃ�q�m�[�h�̊m�F���s���i���s���Ă���΁A�����Œ��f�j
	if (auto status = m_Child[m_nCurrentNode]->Execute(sys); status == CAINode::EStatus::FAILURE)
	{	
		// ���s�ł��Ȃ��̂ŏ�����
		m_nCurrentNode = 0;
		return SetStatus(CAINode::EStatus::FAILURE);
	}
	// ���s���͎��ɐi�܂��Ȃ����߁A���ɍs���Ȃ�
	else if(status == CAINode::EStatus::RUNNING)
		return SetStatus(CAINode::EStatus::RUNNING);

	//--- �V�[�P���X
	++m_nCurrentNode;
	// �Ō�܂ŏI����
	if (m_nCurrentNode >= static_cast<int>(m_Child.size()))
	{
		m_nCurrentNode = 0;
		return SetStatus(CAINode::EStatus::SUCCES);
	}

	// �J�n�֐����Ăяo��
	m_Child[m_nCurrentNode]->OnStart(sys);

	// ���s����Ԃ�
	return SetStatus(CAINode::EStatus::RUNNING);
}
