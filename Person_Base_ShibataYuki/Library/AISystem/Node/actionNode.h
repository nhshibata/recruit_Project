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
	public:
		// *@ノード実行関数
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;
	};
}

#endif // !__ACTION_NODE_H__
