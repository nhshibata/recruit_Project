//========================================================
// [aiAgent.cpp]
//------------------
// 作成:2022/11/24
//========================================================

#include <AISystem/System/aiAgent.h>
#include <AISystem/Node/stateNode.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>

using namespace MySpace::Game;
using namespace AI;

//==========================================================
// コンストラクタ
//==========================================================
CAIAgent::CAIAgent()
{
}

//==========================================================
// コンストラクタ
//==========================================================
CAIAgent::CAIAgent(std::shared_ptr<CGameObject> owner)
	:CAISystem(owner), m_vTarget(owner->GetTransform()->GetPos()), m_fDistans(0), m_fLimitDistans(1),
	m_bIsStoping(false)
{
}

//==========================================================
// デストラクタ
//==========================================================
CAIAgent::~CAIAgent()
{

}

//==========================================================
// ノード更新
//==========================================================
int CAIAgent::NodeUpdate()
{
	auto pos = GetOwner()->GetTransform()->GetPos();
	// 距離の判定
	m_fDistans = sqrtf(pow(m_vTarget.x - pos.x, 2) +
					   pow(m_vTarget.y - pos.y, 2) +
					   pow(m_vTarget.z - pos.z, 2));

	// ノードの更新、確認
	CAISystem::NodeUpdate();

	// 停止命令がでていればやめる
	//if (m_bIsStoping)
	//	return;

	//// 指定距離より近ければ更新しない
	//if (m_fDistans < m_fLimitDistans)
	//	return;

	//// 移動
	//const float OneFram = 0.005f;
	//XMFLOAT3 move;
	//{
	//	XMFLOAT3 ownerPos = m_pOwner->GetPos();
	//	XMFLOAT3 tarPos = GetTarget();
	//	move = XMFLOAT3(tarPos.x - ownerPos.x, tarPos.y - ownerPos.y, tarPos.z - ownerPos.z);
	//	ownerPos.x = ownerPos.x * (1.0f - OneFram) + (ownerPos.x + move.x) * OneFram;
	//	ownerPos.y = ownerPos.y * (1.0f - OneFram) + (ownerPos.y + move.y) * OneFram;
	//	ownerPos.z = ownerPos.z * (1.0f - OneFram) + (ownerPos.z + move.z) * OneFram;
	//	m_pOwner->SetPos(ownerPos);
	//}

	//// LookAt 角度を合わせる
	//XMFLOAT3 angle = m_pOwner->GetAngle();
	//float newAngle = XMConvertToDegrees(atan2(move.x, move.z));
	//angle.y = newAngle;
	//m_pOwner->SetAngle(angle);
	return 0;
}