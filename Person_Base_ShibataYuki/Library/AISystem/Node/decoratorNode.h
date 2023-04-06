//========================================================
// [decoratorNode.h]
//------------------
// �쐬:2022/12/04
//------------------
// �����m�[�h(����)
// Decorator�p�^�[��
//========================================================

#ifndef __DECORATOR_NODE_H__
#define __DECORATOR_NODE_H__

//--- �C���N���[�h��
#include <AISystem/Node/aiNode.h>

//--- �萔��`
#define AID_DEFINITION_IS_ACTION				bool IsExecute(AI::CAISystem* sys)			// ���s������`
#define AID_IMPLEMENTATION_IS_ACTION(type)		bool type##::IsExecute(AI::CAISystem* sys)	// ���s��������

namespace AI
{
	// ���s�m�[�h��K���ݒ�
	class CDecoratorNode : public CAINode
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("decoratorNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pConcrete)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("decoratorNode", cereal::base_class<CAINode>(this)),
					CEREAL_NVP(m_pConcrete)
			);
		}
#pragma endregion

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

CEREAL_REGISTER_TYPE(AI::CDecoratorNode)

#endif // !__DECORATOR_NODE_H__
