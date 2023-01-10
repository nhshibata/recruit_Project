//========================================================
// [SequencerNode.cpp]
//------------------
// �쐬:2022/12/02
//------------------
// �q�m�[�h�̂����ꂩ�����s����
// ���s�ł���Ώ������I����
//========================================================

//--- �C���N���[�h��
#include <AISystem/Node/selectorNode.h>

using namespace AI;

//==========================================================
// �m�[�h���s�֐�
//==========================================================
CAINode::EStatus CSelectorNode::Execute(CAISystem* sys)
{
	// �����ꂩ�����s�����Ȃ�true
	for (auto & child : m_Child)
	{
		//--- ���s
		// ���q�m�[�h��ParentNode�Ȃ�q�m�[�h�̊m�F���s���i�������Ă���΁A�����Œ��f�j
		if (auto status = child->Execute(sys); status == CAINode::EStatus::SUCCES)
			return SetStatus(CAINode::EStatus::SUCCES);
		
		// ���s��
		else if (status == CAINode::EStatus::RUNNING)
			return SetStatus(CAINode::EStatus::RUNNING);
	}

	// ���s�ł�����̂��Ȃ�����
	return SetStatus(CAINode::EStatus::FAILURE);
}