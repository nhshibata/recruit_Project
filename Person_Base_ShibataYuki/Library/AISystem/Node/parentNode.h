//========================================================
// [parentNode.h]
// 作成:2022/12/01
//------------------
// SequencerとSelectorの基となるｸﾗｽ
// 子ノードを持てる
// 子ノードの呼び出しは行うが、変更などは行わない
//========================================================

#ifndef __PARENT_NODE_H__
#define __PARENT_NODE_H__

//--- インクルード部
#include <AISystem/Node/aiNode.h>
#include <vector>

namespace AI
{
	class CParentNode : public CAINode
	{
	protected:
		//--- エイリアス
		using ChildNode = std::vector<CAINode::Ptr>;

	protected:
		//--- メンバ変数
		ChildNode m_Child;

	public:
		//--- メンバ関数

		// *@確認
		virtual bool IsTransition(CAISystem* sys) { return true; };

		// *@ノード実行関数
		virtual CAINode::EStatus Execute(CAISystem* sys) { return CAINode::EStatus::NONE_ACTIVE; };

		// *@子ノードの追加
		inline int AddNode(CAINode::Ptr node)
		{
			m_Child.push_back(node);
			return static_cast<int>(m_Child.size());
		}
	};

}

#endif // !__PARENT_NODE_H__
