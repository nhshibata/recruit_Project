//========================================================
// [AISystem.cpp]
//------------------
// �쐬:2022/11/22
//========================================================

//--- �C���N���[�h��
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
	{	// ���L��؂�
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

	//--- ���s���ȊO�̑S�m�[�h��ҋ@��Ԃɐݒ�
	for (auto & node : m_Nodes)
	{
		if (node.second->GetStatus() == CAINode::EStatus::RUNNING)
			continue;
		node.second->SetStatus(CAINode::EStatus::NONE_ACTIVE);
	}

	// �m�[�h�����s
	// ���򂵂Ă���Ȃ�K�X���s
	if(m_pRoot)
		return static_cast<int>(m_pRoot->Execute(this));
	
	return 0;
}

CAISystem::NodePtr CAISystem::AddNode(NodePtr node)
{
	// �m�[�h�ǉ�
	m_Nodes.insert(std::make_pair(node->GetIndex() ,node));
	// ���ݒ�Ȃ�ݒ�
	if (!m_pRoot)
		m_pRoot = node;
	return node;
}