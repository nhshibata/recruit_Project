//=========================================================
// [partner.h]
// �쐬:2022/12/15
//---------------------------------------------------------
// �p�[�g�i�[���׽
//=========================================================

#ifndef __PARTNER_NODE_H__
#define __PARTNER_NODE_H__

//--- �C���N���[�h��
#include <AISystem/nodeUtil.h>

namespace Spell
{

	class CIsEntryNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};

	class CIsTimerNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};

	class CIsPreferenceNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	class CIsEnvyNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	class CIsPassNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	class CIsMoveNode : public AI::CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{
			this->SetConcreate(sys->MakeNode<CMoveNode>());
		}
		AID_DEFINITION_IS_ACTION;
	};
	
	//--- ���s�m�[�h
	class CEnvyAuthorityNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};
	
	class CAtkAuthorityNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};
	
	// ���ی�
	class CVetoDiscardNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};

	// �ړ�
	class CMoveNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};
	
	// ��~
	class CStopNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};

}

#endif // !__PARTNER_NODE_H__
