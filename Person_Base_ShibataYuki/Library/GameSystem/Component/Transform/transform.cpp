//=========================================================
//作成:2022/04/19 (火曜日)
// トランスフォーム : 部品ｸﾗｽ
//=========================================================

//--- インクルード部
#include <math.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

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
	m_pChilds = std::vector<std::weak_ptr<CTransform>>();
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}
CTransform::CTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_pChilds(0),m_vPos(0,0,0), m_vRot(0,0,0), m_vScale(1,1,1), m_vDestRot(0, 0, 0)
{
	m_pChilds = std::vector<std::weak_ptr<CTransform>>();
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}
CTransform::~CTransform()
{
}
void CTransform::Uninit()
{
	std::vector<std::weak_ptr<CTransform>>::iterator it = m_pChilds.begin();
	for (; it != m_pChilds.end(); ++it)
	{
		auto obj = (*it).lock();
		if (obj) 
		{
			obj->GetOwner()->SetState(CGameObject::E_ObjectState::DESTROY);
		}
	}
}
void CTransform::Init()
{

}
void CTransform::Update()
{
	//--- 過去座標格納
	if (m_vOldPos != m_vPos)
	{
		m_vOldPos = m_vPos;
	}
	//--- マトリックスの更新
	UpdateWorldMatrix();
}
Quaternion CTransform::GetWorldQuaternion()
{
	return m_Rot;
}
Quaternion CTransform::GetLocalQuaternion()
{
	return m_Rot;
}
void CTransform::UpdateWorldMatrix()
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
	// 親が居る場合、親が更新する
	if (auto parent = GetParent(); !parent.lock())
		XMStoreFloat4x4(&m_mWorldMtx, mtx);
	XMStoreFloat4x4(&m_mLocalMtx, mtx);

	// 子が居たら
	if (GetChildCount() == 0)
		return;

	// 子の更新
	auto world = m_mWorldMtx;
	CGameObject::PtrWeak child;
	for (auto child : GetChilds())
	{
		UpdateChildMatrix(child.lock().get(), world);
	}
}
void CTransform::SetWorldMatrix(Vector3 trans, Vector3 rotate, Vector3 scale)
{
	m_vPos = trans;
	m_vRot = rotate;
	m_vScale = scale;

	//UpdateWorldMatrix();
}
void CTransform::SetWorldQuaternion(const Quaternion &  rotation)
{

}
void CTransform::SetLocalQuaternion(const Quaternion &  rotation)
{

}

void CTransform::UpdateChildMatrix(CTransform* child, Matrix4x4 mtx)
{
	// 子のワールドマトリックス更新
	child->UpdateWorldMatrix();

	Matrix4x4 childMtx;
	XMStoreFloat4x4(&childMtx, XMMatrixMultiply(
		XMLoadFloat4x4(&child->GetLocalMatrix()),
		XMLoadFloat4x4(&mtx)
	));
	child->SetWorldMatrix(childMtx);

	// 孫の更新
	int size = child->GetChildCount();
	std::weak_ptr<CTransform> childT;

	for (int cnt = 0; cnt < size; cnt++)
	{
		childT = child->GetChild(cnt);
		if (childT.lock())
		{	// 再帰
			// 子のマトリックスから孫のマトリックスを更新する
			UpdateChildMatrix(childT.lock().get(), childMtx);
		}
	}
	
}

void CTransform::AddChild(std::weak_ptr<CTransform> child)
{
	// ﾎﾟｲﾝﾀ
	if (child.lock() == GetPtr().lock())
		return;
	// 格納済みか
	for (const auto & com : m_pChilds)
	{
		if (child.lock() == com.lock())
			break;
	}
	// 既に親が居れば親子解除
	if (auto parent = child.lock()->GetParent(); parent.lock())
	{
		parent.lock()->RemoveChild(this->BaseToDerived<CTransform>());
	}

	m_pChilds.push_back(child);
	child.lock()->SetParent(this->BaseToDerived<CTransform>());
	Matrix4x4 childMtx;
	XMStoreFloat4x4(&childMtx, XMMatrixMultiply(
		XMLoadFloat4x4(&child.lock()->GetWorldMatrix()),
		XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_mWorldMtx))
	));

	child.lock()->SetLocalMatrix(childMtx);
}
std::weak_ptr<CTransform> CTransform::GetChild(int no)
{
	if (no >= m_pChilds.size())
		return std::shared_ptr<CTransform>();
	return m_pChilds[no].lock();
}

#ifdef BUILD_MODE

void CTransform::ImGuiDebug()
{
	// 3次元座標
	ImGui::DragFloat3(u8"座標", (float*)&m_vPos);
	ImGui::Text(u8"過去座標 %f %f %f", m_vOldPos.x, m_vOldPos.y, m_vOldPos.z);
	ImGui::DragFloat3(u8"角度", (float*)&m_vRot);
	ImGui::DragFloat3(u8"サイズ", (float*)&m_vScale);
	
	//UpdateWorldMatrix();
}
#endif // BUILD_MODE