//========================================================
// [SequencerNode.h]
//------------------
// 作成:2022/12/02
//------------------
// 子ノードを遷移する
// 途中で失敗が帰れば中断
//========================================================

#ifndef __SEQUENCER_NODE_H__
#define __SEQUENCER_NODE_H__

//--- インクルード部
#include <AISystem/Node/parentNode.h>

namespace AI
{
	class CSequencerNode : public CParentNode
	{
	private:
		//--- メンバ変数
		int m_nCurrentNode = 0;

	public:
		//--- メンバ関数
		// *@ノード実行関数
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

#endif // !__SEQUENCER_NODE_H__
