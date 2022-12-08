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
#include "parentNode.h"

namespace AI
{
	class CSelectorNode : public CParentNode
	{
	private:

	public:

		// *@ノード実行関数
		CAINode::EStatus Execute(CAISystem* sys)final;
	};
}

#endif // !__SELECTOR_NODE_H__