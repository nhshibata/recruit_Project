//=========================================================
// [Transform.cpp]
//---------------------------------------------------------
//作成:2022/04/19
//---------------------------------------------------------
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

//==========================================================
// コンストラクタ
//==========================================================
CTransform::CTransform()
{
	m_pChilds = std::vector<std::weak_ptr<CTransform>>();
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CTransform::CTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_pChilds(0),m_vPos(0,0,0), m_vRot(0,0,0), m_vScale(1,1,1), m_vDestRot(0, 0, 0)
{
	m_pChilds = std::vector<std::weak_ptr<CTransform>>();
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}

//==========================================================
// デストラクタ
//==========================================================
CTransform::~CTransform()
{
}

//==========================================================
// 解放
//==========================================================
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

//==========================================================
// 初期化
//==========================================================
void CTransform::Init()
{

}

//==========================================================
// 更新処理
//==========================================================
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

//==========================================================
// 未実装
//==========================================================
Quaternion CTransform::GetWorldQuaternion()
{
	return m_Rot;
}
// 未実装
Quaternion CTransform::GetLocalQuaternion()
{
	return m_Rot;
}

//==========================================================
// マトリックス更新
//==========================================================
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

//==========================================================
// ﾛｰｶﾙ更新
//==========================================================
Matrix4x4 CTransform::CalcLocalMatrix()
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
	
	Matrix4x4 ret;
	XMStoreFloat4x4(&ret, mtx);
	return ret;
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

//==========================================================
// 子オブジェクトの更新
//==========================================================
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

//==========================================================
// 子の追加
//==========================================================
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

//==========================================================
// 子の取得
//==========================================================
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
	Debug::SetTextAndAligned("Transform:OldPos");
	ImGui::Text("%f %f %f", m_vOldPos.x, m_vOldPos.y, m_vOldPos.z);

	Debug::SetTextAndAligned("Transform:Pos");
	ImGui::DragFloat3("##T:Pos", (float*)&m_vPos);

	Debug::SetTextAndAligned("Transform:Rot");
	ImGui::DragFloat3("##T:Rot", (float*)&m_vRot, 1.0f, -360.0f, 360.0f);
	
	Debug::SetTextAndAligned("Transform:Scale");
	ImGui::DragFloat3("##T:Scale", (float*)&m_vScale);
	
}

#endif // BUILD_MODE