//=========================================================
// [partner.cpp]
// 作成:2022/12/15
//---------------------------------------------------------
// パートナー基底ｸﾗｽ
//=========================================================

//--- インクルード部
#include <partner.h>
#include <partnerNode.h>

namespace Spell
{
	//--- デコレーターノード
	AID_IMPLEMENTATION_IS_ACTION(CIsEntryNode)
	{
		return true;
	}
	
	AID_IMPLEMENTATION_IS_ACTION(CIsTimerNode)
	{
		return false;

	}

	AID_IMPLEMENTATION_IS_ACTION(CIsPreferenceNode)
	{

		return false;
	}
	
	AID_IMPLEMENTATION_IS_ACTION(CIsEnvyNode)
	{

		return false;
	}
	
	AID_IMPLEMENTATION_IS_ACTION(CIsPassNode)
	{

		return false;
	}

	//--- 実行ノード
	AID_IMPLEMENTATION_ACTION(CEnvyAuthorityNode)
	{

		return SetStatus(AI::CAINode::EStatus::FAILURE);
	}

	AID_IMPLEMENTATION_ACTION(CAtkAuthorityNode)
	{

		return SetStatus(AI::CAINode::EStatus::FAILURE);
	}

	AID_IMPLEMENTATION_ACTION(CVetoDiscardNode)
	{
		auto param = sys->ConvertData<STPartnerParam>();
		param->eState = EPartnerState::NONE;

		return SetStatus(AI::CAINode::EStatus::FAILURE);
	}
}
