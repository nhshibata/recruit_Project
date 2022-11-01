//=========================================================
//作成:2022/04/19 (火曜日)
// トランスフォーム : 部品ｸﾗｽ
//=========================================================

//--- インクルード部
#include <math.h>
#include <GameSystem/Component/Transform/transform.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::MyMath;

// 名前空間
namespace
{
	const float RATE_ROTATE = 0.2f;
}

// コンストラクタ
CTransform::CTransform()
	:m_pChilds(0),m_vPos(0,0,20), m_vRot(0,0,0), m_vScale(1,1,1),m_vDestRot(0, 0, 0)
{
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}
CTransform::CTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_pChilds(0),m_vPos(0,0,0), m_vRot(0,0,0), m_vScale(1,1,1), m_vDestRot(0, 0, 0)
{
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}
CTransform::CTransform(std::shared_ptr<CGameObject> owner, Vector3 pos, Vector3 rot, Vector3 size)
	: CComponent(owner), m_pChilds(0), m_vPos(pos), m_vRot(rot), m_vScale(size), m_vDestRot(0, 0, 0)
{
	
}
CTransform::~CTransform()
{
}
void CTransform::Uninit()
{
	std::vector<std::weak_ptr<CGameObject>>::iterator it = m_pChilds.begin();
	for (; it != m_pChilds.end(); ++it)
	{
		auto obj = (*it).lock();
		if (obj) 
		{
			obj->SetState(CGameObject::E_ObjectState::DESTROY);
		}
	}
}
void CTransform::Init()
{

}
void CTransform::Update()
{
	if (m_vOldPos != m_vPos)
	{
		m_vOldPos = m_vPos;
	}

	SetWorldMatrix();
}
Quaternion CTransform::GetWorldQuaternion()
{
	return m_Rot;
}
Quaternion CTransform::GetLocalQuaternion()
{
	return m_Rot;
}
void CTransform::SetWorldMatrix()
{
	XMMATRIX mtx, scl, rot, translate;
	mtx = XMMatrixIdentity();

	// サイズ反映
	scl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtx = XMMatrixMultiply(mtx, scl);

	// 回転
	rot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRot.x), XMConvertToRadians(m_vRot.y),
		XMConvertToRadians(m_vRot.z));
	mtx = XMMatrixMultiply(mtx, rot);

	// 位置を反映
	translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtx = XMMatrixMultiply(mtx, translate);

	// ワールドマトリックスを反映
	XMStoreFloat4x4(&m_mWorldMtx, mtx);
	XMStoreFloat4x4(&m_mLocalMtx, mtx);

	// 親が居たら
	if (auto parent = GetParent(); parent.lock())
	{
		auto world = m_mWorldMtx;
		while (parent.lock())
		{
			auto otherWorld = parent.lock()->GetTransform()->GetWorldMatrix();
			world = world * otherWorld;
			parent = parent.lock()->GetTransform()->GetParent();
		}
		m_mWorldMtx = world;
	}
}

void CTransform::SetWorldQuaternion(const Quaternion &  rotation)
{

}
void CTransform::SetLocalQuaternion(const Quaternion &  rotation)
{

}

std::weak_ptr<CGameObject> CTransform::GetChild(int no)
{
	if (no >= m_pChilds.size())return std::make_shared<CGameObject>(); 
	return m_pChilds[no].lock();
}
#ifdef BUILD_MODE


void CTransform::ImGuiDebug()
{
	//if (ImGui::CollapsingHeader("open"))
	{
		// 3次元座標
		//ImGui::InputFloat3(u8"プレイヤー座標", (float*)&this->GetPos());
		ImGui::InputFloat3(u8"座標", (float*)m_vPos);
	//	m_vPos = Debug::InputInt(m_vPos);
		/*ImGui::InputInt(u8"座標x", (int*)&m_vPos.x);
		ImGui::InputInt(u8"座標y", (int*)&m_vPos.y);
		ImGui::InputInt(u8"座標z", (int*)&m_vPos.z);*/
		ImGui::Text(u8"過去座標 %f %f %f", m_vOldPos.x, m_vOldPos.y, m_vOldPos.z);
		
		ImGui::InputFloat3(u8"サイズ", (float*)m_vScale);
	//	m_vScale = Debug::InputInt(m_vScale);
		/*ImGui::InputInt(u8"サイズx", (int*)&m_vScale.x);
		ImGui::InputInt(u8"サイズy", (int*)&m_vScale.y);
		ImGui::InputInt(u8"サイズz", (int*)&m_vScale.z);*/

		ImGui::InputFloat3(u8"角度", (float*)m_vRot);
	//	m_vRot = Debug::InputInt(m_vRot);
	/*	ImGui::InputInt(u8"角度x", (int*)&m_vRot.x);
		ImGui::InputInt(u8"角度y", (int*)&m_vRot.y);
		ImGui::InputInt(u8"角度z", (int*)&m_vRot.z);*/
	}
}
#endif // BUILD_MODE