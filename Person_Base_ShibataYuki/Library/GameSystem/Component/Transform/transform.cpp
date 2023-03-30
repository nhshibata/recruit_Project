//=========================================================
// [Transform.cpp]
//---------------------------------------------------------
//�쐬:2022/04/19
//---------------------------------------------------------
// �g�����X�t�H�[�� : ���i�׽
//=========================================================

//--- �C���N���[�h��
#include <math.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::MyMath;

//==========================================================
// �R���X�g���N�^
//==========================================================
CTransform::CTransform()
{
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CTransform::CTransform(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_pChilds(0),m_vPos(0,0,0), m_vRot(0,0,0), m_vScale(1,1,1), m_vDestRot(0, 0, 0)
{	
	XMStoreFloat4x4(&m_mLocalMtx, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWorldMtx, XMMatrixIdentity());
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CTransform::~CTransform()
{
	// �e�q�֌W����
	ParentDissolved();

	if (GetOwner())
	{
		if (GetOwner()->GetState() != CGameObject::E_ObjectState::DESTROY)
			return;
	}

	// �폜��Ԃŏ����ꂽ�Ƃ�(Uninit�͍폜��ԈȊO�A�ǂݍ��݂Ȃǂł��Ă΂��)
	// �q���폜
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
// ������
//==========================================================
void CTransform::Init()
{

}

//==========================================================
// ���
//==========================================================
void CTransform::Uninit()
{
	
}

//==========================================================
// �X�V����
//==========================================================
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

//==========================================================
// ������
//==========================================================
Quaternion CTransform::GetWorldQuaternion()
{
	return m_Rot;
}

// ������
Quaternion CTransform::GetLocalQuaternion()
{
	return m_Rot;
}

//==========================================================
// �}�g���b�N�X�X�V
//==========================================================
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
	if (!GetParent().lock())
		XMStoreFloat4x4(&m_mWorldMtx, mtx);
	XMStoreFloat4x4(&m_mLocalMtx, mtx);

	// �q�����邩�m�F
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

//==========================================================
// ۰�ٍX�V
//==========================================================
Matrix4x4 CTransform::CalcLocalMatrix()
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
// �q�I�u�W�F�N�g�̍X�V
//==========================================================
void CTransform::UpdateChildMatrix(CTransform* child, Matrix4x4 mtx)
{
	// �q�̃��[���h�}�g���b�N�X�X�V
	child->UpdateWorldMatrix();

	Matrix4x4 childMtx;
	// �q�I�u�W�F�N�g�̃��[�J���s��Ɛe�I�u�W�F�N�g�̃��[���h�s�����Z����
	// ���[���h�s������߂�
	XMStoreFloat4x4(&childMtx, XMMatrixMultiply(
		XMLoadFloat4x4(&child->GetLocalMatrix()),
		XMLoadFloat4x4(&mtx)
	));
	child->SetWorldMatrix(childMtx);

	// �X�Ɏq�̍X�V
	int size = child->GetChildCount();
	std::weak_ptr<CTransform> childT;

	for (int cnt = 0; cnt < size; cnt++)
	{
		childT = child->GetChild(cnt);
		if (childT.lock())
		{	
			// �ċA
			// �q�̃}�g���b�N�X����X�Ɏq�̃}�g���b�N�X���X�V����
			UpdateChildMatrix(childT.lock().get(), childMtx);
		}
	}
	
}

//==========================================================
// �q�̒ǉ�
//==========================================================
void CTransform::AddChild(std::weak_ptr<CTransform> child)
{
	// �������g�ł͂Ȃ����m�F
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

	// �q�ɉ�����
	m_pChilds.push_back(child);
	// �e��������
	child.lock()->SetParent(this->BaseToDerived<CTransform>());

	//--- ܰ��ލ��W�n�ł̎q�I�u�W�F�N�g�̈ʒu�ݒ�
	Matrix4x4 childMtx;
	// ���g�̃��[���h�}�g���b�N�X�̋t�s����q�I�u�W�F�N�g�̍s��Ƃ����A�e�̍��W�n�Ƃ���
	// ����ɂ��A�q�I�u�W�F�N�g�̍��W�n���e�Ƒ��ΓI�ȍ��W�ɂȂ�
	XMStoreFloat4x4(&childMtx, XMMatrixMultiply(
		XMLoadFloat4x4(&child.lock()->GetWorldMatrix()),
		XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_mWorldMtx))
	));

	child.lock()->SetLocalMatrix(childMtx);
}

//==========================================================
// �q�v�f���O
//==========================================================
void CTransform::RemoveChild(std::weak_ptr<CTransform> child)
{
	for (auto it = m_pChilds.begin(); it != m_pChilds.end(); ++it)
	{
		if ((*it).lock() == child.lock())
		{
			m_pChilds.erase(it);
			break;
		}
	}
}

//==========================================================
// �e�q�֌W����
//==========================================================
void CTransform::ParentDissolved()
{
	if (!m_pParent.lock())
		return;
	m_pParent.lock()->RemoveChild(BaseToDerived<CTransform>());
	m_pParent.reset();
}

//==========================================================
// �q�̎擾
//==========================================================
std::weak_ptr<CTransform> CTransform::GetChild(const int& no)
{
	if (no >= m_pChilds.size())
		return std::shared_ptr<CTransform>();
	return m_pChilds[no].lock();
}


#ifdef BUILD_MODE

void CTransform::ImGuiDebug()
{
	// 3�������W
	Debug::SetTextAndAligned("Transform:OldPos");
	ImGui::Text("[x:%.5f][y:%.5f][z:%.5f]", m_vOldPos.x, m_vOldPos.y, m_vOldPos.z);

	Debug::SetTextAndAligned("Transform:Pos");
	ImGui::DragFloat3("##T:Pos", (float*)&m_vPos);

	Debug::SetTextAndAligned("Transform:Rot");
	ImGui::DragFloat3("##T:Rot", (float*)&m_vRot, 1.0f, -360.0f, 360.0f);
	
	Debug::SetTextAndAligned("Transform:Scale");
	ImGui::DragFloat3("##T:Scale", (float*)&m_vScale);
	
}

#endif // BUILD_MODE