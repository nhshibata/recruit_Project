//========================================================
// [stateNode.cpp]
//-------------------
// 作成:2022/11/22
//========================================================

//--- インクルード部
#include <AISystem/Node/stateNode.h>
#include <AISystem/stateCondition.h>
#include <AISystem/System/aiSystem.h>

using namespace AI;

CAINode::EStatus CStateNode::Execute(CAISystem* system)
{
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