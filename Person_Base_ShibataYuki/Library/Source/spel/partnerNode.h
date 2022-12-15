//=========================================================
// [partner.h]
// 作成:2022/12/15
//---------------------------------------------------------
// パートナー基底ｸﾗｽ
//=========================================================

#ifndef __PARTNER_NODE_H__
#define __PARTNER_NODE_H__

//--- インクルード部
#include <AISystem/nodeUtil.h>

namespace Spell
{
	class CIsEntryNode : public CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};

	class CIsTimerNode : public CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};

	class CIsPreferenceNode : public CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	class CIsEnvyNode : public CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	class CIsPassNode : public CDecoratorNode
	{
	public:
		AID_IMPLEMENTATION_AWAKE
		{

		}
		AID_DEFINITION_IS_ACTION;
	};
	
	//--- 実行ノード
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
	
	// 拒否権
	class CVetoDiscardNode : public AI::CActionNode
	{
	public:
		AID_DEFINITION_ACTION;
	};
}

#endif // !__PARTNER_NODE_H__
