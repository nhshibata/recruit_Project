//========================================================
// [actionNode.h]
//------------------
// 作成:2022/12/04
//------------------
// 実行ノード
// 子は持てない
//========================================================

//--- インクルード部
#include <AISystem/Node/aiNode.h>

#ifndef __ACTION_NODE_H__
#define __ACTION_NODE_H__

namespace AI
{
	class CActionNode : public CAINode
	{
#pragma region Serialize
	private:
		//--- シリアライズ
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
		// *@ノード実行関数
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;
	};
}

CEREAL_REGISTER_TYPE(AI::CActionNode)

#endif // !__ACTION_NODE_H__
