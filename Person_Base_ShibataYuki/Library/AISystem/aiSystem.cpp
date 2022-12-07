//========================================================
// [AISystem.cpp]
//------------------
// 作成:2022/11/22
//========================================================

//--- インクルード部
#include "aiSystem.h"
#include "aiNode.h"
#include <GameSystem/GameObject/gameObject.h>

using namespace AI;
using namespace MySpace::Game;

CAISystem::CAISystem(std::shared_ptr<CGameObject> ptr)
	:m_pOwner(ptr), m_pRoot(NodePtr()), m_pData(nullptr)
{
}
CAISystem::~CAISystem()
{	
	for (auto & node : m_Nodes)
	{	// 所有を切る
		node.second.reset();
	}
	m_Nodes.clear();
	delete m_pData;
}

void CAISystem::Init()
{
	for (auto & node : m_Nodes)
	{
		node.second->OnAwake(this);
	}
}
void CAISystem::Update()
{
	NodeUpdate();
}
int CAISystem::NodeUpdate()
{
#if _DEBUG
	m_nNodeCnt = 0;
#endif // _DEBUG

	//--- 実行中以外の全ノードを待機状態に設定
	for (auto & node : m_Nodes)
	{
		if (node.second->GetStatus() == CAINode::EStatus::RUNNING)
			continue;
		node.second->SetStatus(CAINode::EStatus::NONE_ACTIVE);
	}

	// ノードを実行
	// 分岐しているなら適宜実行
	if(m_pRoot)
		return static_cast<int>(m_pRoot->Execute(this));
	
	return 0;
}

CAISystem::NodePtr CAISystem::AddNode(NodePtr node)
{
	// ノード追加
	m_Nodes.insert(std::make_pair(node->GetIndex() ,node));
	// 未設定なら設定
	if (!m_pRoot)
		m_pRoot = node;
	return node;
}