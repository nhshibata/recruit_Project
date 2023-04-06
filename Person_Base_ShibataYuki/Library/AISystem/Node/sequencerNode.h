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
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("seqencerNode", cereal::base_class<CSequencerNode>(this)),
					CEREAL_NVP(m_nCurrentNode)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("seqencerNode", cereal::base_class<CSequencerNode>(this)),
					CEREAL_NVP(m_nCurrentNode)
			);
		}
#pragma endregion
	private:
		//--- �����o�ϐ�
		int m_nCurrentNode = 0;

	public:
		//--- �����o�֐�
		// *@�m�[�h���s�֐�
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

CEREAL_REGISTER_TYPE(AI::CSequencerNode)

#endif // !__SEQUENCER_NODE_H__
