//========================================================
// [actionNode.h]
//------------------
// �쐬:2022/12/04
//------------------
// ���s�m�[�h
// �q�͎��ĂȂ�
//========================================================

//--- �C���N���[�h��
#include <AISystem/Node/aiNode.h>

#ifndef __ACTION_NODE_H__
#define __ACTION_NODE_H__

namespace AI
{
	class CActionNode : public CAINode
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("actionNode", cereal::base_class<CAINode>(this))
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("actionNode", cereal::base_class<CAINode>(this))
			);
		}
#pragma endregion
	public:
		// *@�m�[�h���s�֐�
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;
	};
}

CEREAL_REGISTER_TYPE(AI::CActionNode)

#endif // !__ACTION_NODE_H__
