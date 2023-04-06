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
#include <AISystem/Node/aiNode.h>
#include <vector>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
	class CStateCondition;
#pragma endregion

	class CStateNode : public CAINode
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("stateNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_pConditons)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("stateNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_pConditons)
			);
		}
#pragma endregion
	private:
		//--- �G�C���A�X
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

CEREAL_REGISTER_TYPE(AI::CStateNode)

#endif // !__STATE_NODE_H__

