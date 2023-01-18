//=========================================================
// [partnerNode.cpp]
// 作成:2022/12/15
//---------------------------------------------------------
// パートナーｸﾗｽ
//=========================================================

//--- インクルード部
#include <partner.h>
#include <partnerNode.h>
#include <AISystem/aiFunction.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace AI;

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
	
	AID_IMPLEMENTATION_IS_ACTION(CIsMoveNode)
	{
		auto param = sys->GetNode<STPartnerParamB>();
		param->fStopTime.fValue -= CFps::Get().DeltaTime();
		if(param->fStopTime.fValue > 0)
			return false;
		return true;
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

	AID_IMPLEMENTATION_ACTION(CMoveNode)
	{
		auto param = sys->GetNode<STPartnerParamB>();
		auto trans = sys->GetOwner()->GetTransform();
		Vector3 targetPos;
		// 座標取得
		param->nPosIdx = param->m_pSuppo.lock()->Bind(param->nPosIdx);
		param->nPosIdx = param->m_pSuppo.lock()->GetPos(param->nPosIdx, targetPos);
		// 取得失敗
		if(param->nPosIdx == -1)
			return SetStatus(AI::CAINode::EStatus::FAILURE);
		// 座標更新
		auto newPos = AI::Smoothly(trans->GetPos(), targetPos);
		trans->SetPos(newPos);

		// 距離確認
		if(AI::GetDistance(newPos, targetPos) < param->fDistance)
			return SetStatus(AI::CAINode::EStatus::SUCCES);

		return SetStatus(AI::CAINode::EStatus::RUNNING);
	}
	
	// 停止
	AID_IMPLEMENTATION_ACTION(CStopNode)
	{
		auto param = sys->GetNode<STPartnerParamB>();
		param->fStopTime.fValue = param->fStopTime.fMax;
		param->m_pSuppo.lock()->Release(param->nPosIdx);
		return SetStatus(AI::CAINode::EStatus::SUCCES);
	}
}
