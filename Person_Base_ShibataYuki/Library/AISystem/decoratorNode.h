//========================================================
// [decoratorNode.h]
//------------------
// �쐬:2022/12/04
//------------------
// �����m�[�h(����)
// Decorator�p�^�[��
//========================================================

//--- �C���N���[�h��
#include "aiNode.h"

#ifndef __DECORATOR_NODE_H__
#define __DECORATOR_NODE_H__

#define AID_DEFINITION_IS_ACTION				bool IsExecute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_IS_ACTION(type)		bool type##::IsExecute(AI::CAISystem* sys)

namespace AI
{
	class CDecoratorNode : public CAINode
	{
	private:
		//--- �����o�ϐ�
		CAINode::Ptr m_pConcrete;		// �����Ώ�

	public:
		//--- �����o�֐�

		// *@�����m�F
		// *@�I�[�o�[���C�h���āA���s����������L�q
		virtual bool IsExecute(CAISystem* sys) = 0;

		// *@�m�[�h���s�֐�
		CAINode::EStatus Execute(CAISystem* sys)
		{
			if(!IsExecute(sys))
				return CAINode::EStatus::FAILURE;
			// ���s
			//if(m_pConcrete)
			return m_pConcrete->Execute(sys);
			
			return CAINode::EStatus::FAILURE;
		}

		// *@�C���Ώېݒ�
		inline void SetConcreate(CAINode::Ptr ptr) { m_pConcrete = ptr; }
	};
}

#endif // !__DECORATOR_NODE_H__
