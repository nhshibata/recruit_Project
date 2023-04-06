//========================================================
// [SequencerNode.h]
//------------------
// 作成:2022/12/02
//------------------
// 子ノードのいずれかを実行する
// 実行できれば処理を終える
//========================================================

#ifndef __SELECTOR_NODE_H__
#define __SELECTOR_NODE_H__

//--- インクルード部
#include <AISystem/Node/parentNode.h>

namespace AI
{
	class CSelectorNode : public CParentNode
	{
#pragma region Serialize
	private:
		//--- シリアライズ
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
		// *@ノード実行関数
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

CEREAL_REGISTER_TYPE(AI::CSelectorNode)

#endif // !__SELECTOR_NODE_H__