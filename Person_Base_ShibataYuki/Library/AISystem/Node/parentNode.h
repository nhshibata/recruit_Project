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
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("parentNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_Child)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("parentNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_Child)
			);
		}
#pragma endregion
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

CEREAL_REGISTER_TYPE(AI::CParentNode)

#endif // !__PARENT_NODE_H__
