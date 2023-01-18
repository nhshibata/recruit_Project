//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �g�����X�t�H�[�� : ���i�׽
//=========================================================

//--- �C���N���[�h��
#include <math.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::MyMath;

// ���O���
namespace
{
	const float RATE_ROTATE = 0.2f;
}

// �R���X�g���N�^
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
	//--- �ߋ����W�i�[
	if (m_vOldPos != m_vPos)
	{
		m_vOldPos = m_vPos;
	}
	//--- �}�g���b�N�X�̍X�V
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

	// �T�C�Y���f
	scl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtx = XMMatrixMultiply(mtx, scl);

	// ��]
	rot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vRot.x), XMConvertToRadians(m_vRot.y),
		XMConvertToRadians(m_vRot.z));
	mtx = XMMatrixMultiply(mtx, rot);

	// �ʒu�𔽉f
	translate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtx = XMMatrixMultiply(mtx, translate);

	// ���[���h�}�g���b�N�X�𔽉f
	// �e������ꍇ�A�e���X�V����
	if (auto parent = GetParent(); !parent.lock())
		XMStoreFloat4x4(&m_mWorldMtx, mtx);
	XMStoreFloat4x4(&m_mLocalMtx, mtx);

	// �q��������
	if (GetChildCount() == 0)
		return;

	// �q�̍X�V
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
	// �q�̃��[���h�}�g���b�N�X�X�V
	child->UpdateWorldMatrix();

	Matrix4x4 childMtx;
	XMStoreFloat4x4(&childMtx, XMMatrixMultiply(
		XMLoadFloat4x4(&child->GetLocalMatrix()),
		XMLoadFloat4x4(&mtx)
	));
	child->SetWorldMatrix(childMtx);

	// ���̍X�V
	int size = child->GetChildCount();
	std::weak_ptr<CTransform> childT;

	for (int cnt = 0; cnt < size; cnt++)
	{
		childT = child->GetChild(cnt);
		if (childT.lock())
		{	// �ċA
			// �q�̃}�g���b�N�X���瑷�̃}�g���b�N�X���X�V����
			UpdateChildMatrix(childT.lock().get(), childMtx);
		}
	}
	
}

void CTransform::AddChild(std::weak_ptr<CTransform> child)
{
	// �߲��
	if (child.lock() == GetPtr().lock())
		return;
	// �i�[�ς݂�
	for (const auto & com : m_pChilds)
	{
		if (child.lock() == com.lock())
			break;
	}
	// ���ɐe������ΐe�q����
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
	// 3�������W
	ImGui::DragFloat3(u8"���W", (float*)&m_vPos);
	ImGui::Text(u8"�ߋ����W %f %f %f", m_vOldPos.x, m_vOldPos.y, m_vOldPos.z);
	ImGui::DragFloat3(u8"�p�x", (float*)&m_vRot);
	ImGui::DragFloat3(u8"�T�C�Y", (float*)&m_vScale);
	
	//UpdateWorldMatrix();
}
#endif // BUILD_MODE