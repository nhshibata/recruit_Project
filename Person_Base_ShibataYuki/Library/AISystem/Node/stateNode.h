//========================================================
// [stateNode.h]
//-------------------
// �쐬:2022/11/22
//-------------------
// �X�e�[�g�ؑ�
// 
//========================================================

#ifndef __STATE_NODE_H__
#define __STATE_NODE_H__

//--- �C���N���[�h��
#include "aiNode.h"
#include <vector>

namespace AI
{
	//--- �O���錾
	class CAISystem;
	class CStateCondition;

	class CStateNode : public CAINode
	{
	private:
		using StateCondition = std::shared_ptr<CStateCondition>;
		using StateMachine = std::vector<StateCondition>;

	private:
		//--- �����o�ϐ�
		CAINode::Ptr m_pRoot;
		StateMachine m_pConditons;

	public:
		//--- �����o�֐�

		// *@�m�[�h���s�֐�
		// *@������I�[�o�[���C�h���邱��
		virtual CAINode::EStatus Update(CAISystem* system);

		// *@���s�m�F
		// *@�q���������ċA
		CAINode::EStatus Execute(CAISystem* system);

		// *@�J�ڒǉ�
		inline void AddCondition(StateCondition add){ m_pConditons.push_back(add); }

		// *@�m�[�h�̐ݒ�
		inline void SetNode(CAINode::Ptr node) { m_pRoot = node; }
	};
}

#endif // !__STATE_NODE_H__

