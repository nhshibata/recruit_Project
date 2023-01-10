//========================================================
// [SequencerNode.cpp]
//------------------
// 作成:2022/12/02
//------------------
// 子ノードのいずれかを実行する
// 実行できれば処理を終える
//========================================================

//--- インクルード部
#include <AISystem/Node/selectorNode.h>

using namespace AI;

//==========================================================
// ノード実行関数
//==========================================================
CAINode::EStatus CSelectorNode::Execute(CAISystem* sys)
{
	// いずれかが実行したならtrue
	for (auto & child : m_Child)
	{
		//--- 実行
		// ※子ノードがParentNodeなら子ノードの確認を行う（成功していれば、ここで中断）
		if (auto status = child->Execute(sys); status == CAINode::EStatus::SUCCES)
			return SetStatus(CAINode::EStatus::SUCCES);
		
		// 実行中
		else if (status == CAINode::EStatus::RUNNING)
			return SetStatus(CAINode::EStatus::RUNNING);
	}

	// 実行できるものがなかった
	return SetStatus(CAINode::EStatus::FAILURE);
}