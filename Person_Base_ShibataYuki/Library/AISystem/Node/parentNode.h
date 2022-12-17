//========================================================
// [parentNode.h]
// �쐬:2022/12/01
//------------------
// Sequencer��Selector�̊�ƂȂ�׽
// �q�m�[�h�����Ă�
// �q�m�[�h�̌Ăяo���͍s�����A�ύX�Ȃǂ͍s��Ȃ�
//========================================================

#ifndef __PARENT_NODE_H__
#define __PARENT_NODE_H__

//--- �C���N���[�h��
#include <AISystem/Node/aiNode.h>
#include <vector>

namespace AI
{
	class CParentNode : public CAINode
	{
	protected:
		//--- �G�C���A�X
		using ChildNode = std::vector<CAINode::Ptr>;

	protected:
		//--- �����o�ϐ�
		ChildNode m_Child;

	public:
		//--- �����o�֐�

		// *@�m�F
		virtual bool IsTransition(CAISystem* sys) { return true; };

		// *@�m�[�h���s�֐�
		virtual CAINode::EStatus Execute(CAISystem* sys) { return CAINode::EStatus::NONE_ACTIVE; };

		// *@�q�m�[�h�̒ǉ�
		inline int AddNode(CAINode::Ptr node)
		{
			m_Child.push_back(node);
			return static_cast<int>(m_Child.size());
		}
	};

}

#endif // !__PARENT_NODE_H__
