//=========================================================
// [partnerNode.cpp]
// �쐬:2022/12/15
//---------------------------------------------------------
// �p�[�g�i�[�׽
//=========================================================

//--- �C���N���[�h��
#include <partner.h>
#include <partnerNode.h>
#include <AISystem/aiFunction.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace AI;

namespace Spell
{
	//--- �f�R���[�^�[�m�[�h
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

	//--- ���s�m�[�h
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
		// ���W�擾
		param->nPosIdx = param->m_pSuppo.lock()->Bind(param->nPosIdx);
		param->nPosIdx = param->m_pSuppo.lock()->GetPos(param->nPosIdx, targetPos);
		// �擾���s
		if(param->nPosIdx == -1)
			return SetStatus(AI::CAINode::EStatus::FAILURE);
		// ���W�X�V
		auto newPos = AI::Smoothly(trans->GetPos(), targetPos);
		trans->SetPos(newPos);

		// �����m�F
		if(AI::GetDistance(newPos, targetPos) < param->fDistance)
			return SetStatus(AI::CAINode::EStatus::SUCCES);

		return SetStatus(AI::CAINode::EStatus::RUNNING);
	}
	
	// ��~
	AID_IMPLEMENTATION_ACTION(CStopNode)
	{
		auto param = sys->GetNode<STPartnerParamB>();
		param->fStopTime.fValue = param->fStopTime.fMax;
		param->m_pSuppo.lock()->Release(param->nPosIdx);
		return SetStatus(AI::CAINode::EStatus::SUCCES);
	}
}
