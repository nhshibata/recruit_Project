//========================================================
// [SequencerNode.cpp]
//------------------
// 作成:2022/12/02
//------------------
// 子ノードを遷移する
// 途中で失敗が帰れば中断
//========================================================

//--- インクルード部
#include "sequencerNode.h"

using namespace AI;

CAINode::EStatus CSequencerNode::Execute(CAISystem* sys)
{
	//--- 現在ノードの実行
	// ※子ノードがParentNodeなら子ノードの確認を行う（失敗していれば、ここで中断）
	if (auto status = m_Child[m_nCurrentNode]->Execute(sys); status == CAINode::EStatus::FAILURE)
	{	
		// 実行できないので初期化
		m_nCurrentNode = 0;
		return SetStatus(CAINode::EStatus::FAILURE);
	}
	// 実行中は次に進ませないため、下に行かない
	else if(status == CAINode::EStatus::RUNNING)
		return SetStatus(CAINode::EStatus::RUNNING);

	//--- シーケンス
	++m_nCurrentNode;
	// 最後まで終えた
	if (m_nCurrentNode >= static_cast<int>(m_Child.size()))
	{
		m_nCurrentNode = 0;
		return SetStatus(CAINode::EStatus::SUCCES);
	}

	// 開始関数を呼び出す
	m_Child[m_nCurrentNode]->OnStart(sys);

	// 実行中を返す
	return SetStatus(CAINode::EStatus::RUNNING);
}
