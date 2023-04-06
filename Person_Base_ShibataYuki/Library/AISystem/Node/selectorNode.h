//========================================================
// [SequencerNode.h]
//------------------
// �쐬:2022/12/02
//------------------
// �q�m�[�h�̂����ꂩ�����s����
// ���s�ł���Ώ������I����
//========================================================

#ifndef __SELECTOR_NODE_H__
#define __SELECTOR_NODE_H__

//--- �C���N���[�h��
#include <AISystem/Node/parentNode.h>

namespace AI
{
	class CSelectorNode : public CParentNode
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("selectorNode", cereal::base_class<CSelectorNode>(this))
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("selectorNode", cereal::base_class<CSelectorNode>(this))
			);
		}
#pragma endregion

	public:
		// *@�m�[�h���s�֐�
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

CEREAL_REGISTER_TYPE(AI::CSelectorNode)

#endif // !__SELECTOR_NODE_H__