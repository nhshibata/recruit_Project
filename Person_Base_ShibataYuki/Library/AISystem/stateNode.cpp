//========================================================
// [stateNode.cpp]
//-------------------
// 作成:2022/11/22
//========================================================

//--- インクルード部
#include "stateNode.h"
#include "stateCondition.h"

#include "aiSystem.h"
#if _DEBUG
#endif

using namespace AI;

CAINode::EStatus CStateNode::Execute(CAISystem* system)
{
#if _DEBUG
	++system->m_nNodeCnt;
#endif // _DEBUG

	//--- 遷移確認
	CAINode::Ptr next = CAINode::Ptr();
	for (auto & condition : m_pConditons)
	{	// 条件が満たしていれば、状態変更
		next = condition->CheckTransition(system);
		if (next)
		{
			system->SetRoot(next);
			break;
		}
	}

	//--- 自身の更新
	// 所有ノードの実行を行うが、オーバーライドされていた場合は異なる
	return Update(system);
}

CAINode::EStatus CStateNode::Update(CAISystem* system)
{
	//--- 所有ノードの実行
	// ※未設定ならエラー
	return m_pRoot->Execute(system);
}