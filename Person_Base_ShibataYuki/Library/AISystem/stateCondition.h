//========================================================
// [stateCondition.h]
//------------------
// �쐬:2022/12/04
//------------------
// ���s�m�[�h
// �q�͎��ĂȂ�
//========================================================

#ifndef __STATE_CONDITION_H__
#define __STATE_CONDITION_H__

//--- �C���N���[�h��
#include "aiCondition.h"

namespace AI
{
	class CAISystem;
	class CAINode;
	class CStateNode;

	class CStateCondition
	{
	private:
		using NextNode = std::shared_ptr<CStateNode>;

	private:
		//--- �����o�ϐ�
		CAICondition::Ptr m_Condition;
		NextNode m_pNextNode;

	public:
		//--- �����o�֐�
		CStateCondition(NextNode next): m_pNextNode(next)
		{
			m_Condition = std::make_shared<CAICondition>();
		}
		~CStateCondition()
		{}

		// *@��Ԋm�F
		NextNode CheckTransition(CAISystem* system)
		{
			// ���������s�ł����Ԃ��m�F
			if (m_Condition->IsTransition(system))
				return m_pNextNode;

			return NextNode();
		}

		// *@�֐��߲���ǉ�
		inline void AddCondition(CAICondition::FuncPtr func){ m_Condition->AddFunc(func); }

		// *@���ւ̃m�[�h
		inline void SetNextNode(NextNode node) { m_pNextNode = node; }
	};
}

#endif // !__STATE_CONDITION_H__
