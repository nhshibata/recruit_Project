//========================================================
// [SequencerNode.h]
//------------------
// �쐬:2022/12/02
//------------------
// �q�m�[�h��J�ڂ���
// �r���Ŏ��s���A��Β��f
//========================================================

#ifndef __SEQUENCER_NODE_H__
#define __SEQUENCER_NODE_H__

//--- �C���N���[�h��
#include <AISystem/Node/parentNode.h>

namespace AI
{
	class CSequencerNode : public CParentNode
	{
	private:
		//--- �����o�ϐ�
		int m_nCurrentNode = 0;

	public:
		//--- �����o�֐�
		// *@�m�[�h���s�֐�
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

#endif // !__SEQUENCER_NODE_H__
