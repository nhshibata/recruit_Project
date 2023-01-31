//=========================================================
//�쐬:2022/05/02
// �R���W�����׽(�����蔻�� ) : �h���׽
//
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#if BUILD_MODE

#include <ImGui/imgui.h>
#include <GraphicsSystem/Render/box.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>

#endif // BUILD_MODE

using namespace MySpace::Game;

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CBoxCollision::CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size)
	: CCollision(owner),m_vSize(size), m_bOBBMode(true)
{
#if BUILD_MODE

	m_pDebugBox = std::make_shared<CBox>();
	m_pDebugBox->Init(size);
	m_pDebugBox->SetDiffuse(Vector4(0, 1, 0, 0.5f));

#endif // BUILD_MODE
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBoxCollision::~CBoxCollision()
{
#if BUILD_MODE
	m_pDebugBox->Fin();
	m_pDebugBox.reset();
#endif // BUILD_MODE
}

//==========================================================
// �����蔻��֐�
//==========================================================
bool CBoxCollision::Box(Vector3 Apos, Vector3 Asize, Vector3 Bpos, Vector3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

//==========================================================
// ���g�ƈ�������񂩂�̓����蔻��
//==========================================================
bool CBoxCollision::CollisionAABB(Vector3 pos, Vector3 size)
{
	if (Box(Transform()->GetPos(), m_vSize, pos, size))
	{
		return true;
	}
	return false;
}

//==========================================================
// �����蔻��OBB
//==========================================================
bool CBoxCollision::CollisionOBB(CTransform* trans, Vector3 center, Vector3 size)
{
	// ���[���h�}�g���b�N�X�擾
	XMFLOAT4X4 mW1 = Transform()->GetWorldMatrix();
	XMFLOAT4X4 mW2 = trans->GetWorldMatrix();

	// ���S���W�擾
	XMFLOAT3 vP1, vP2;
	XMStoreFloat3(&vP1, XMVector3TransformCoord(XMLoadFloat3(&m_vCenter), XMLoadFloat4x4(&mW1)));
	XMStoreFloat3(&vP2, XMVector3TransformCoord(XMLoadFloat3(&center), XMLoadFloat4x4(&mW2)));

	// ���S���W�Ԃ̃x�N�g�������߂�
	XMVECTOR vD;
	vD = XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);

	// ���f�����W�������߂�
	XMVECTOR vN[6];
	vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
	vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
	vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
	vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);

	// OBB�̑傫��(����)���|�����x�N�g�������߂�
	XMFLOAT3& vBB1 = m_vSize;
	XMFLOAT3& vBB2 = size;
	XMVECTOR vL[6];
	vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
	vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
	vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
	vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB1.x, mW2._13 * vBB2.x, 0.0f);
	vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB1.y, mW2._23 * vBB2.y, 0.0f);
	vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB1.z, mW2._33 * vBB2.z, 0.0f);

	// ��������₪�ʂ̖@���x�N�g��(���f�����W��)
	float fL, f, fD;
	for (int cnt = 0; cnt < 5; ++cnt)
	{
		XMVECTOR& vS = vN[cnt]; // ���������
		// OBB�̉e(�̔���)�̍��v
		fL = 0.0f;
		for (int cntObb = 0; cntObb < 6; ++cntObb)
		{
			XMStoreFloat(&f, XMVector3Dot(vS, vL[cntObb]));
			fL += fabsf(f);
		}

		// ���S���W�Ԃ̋����Ɣ�r
		XMStoreFloat(&fD, XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD)
			return false; // �������Ă��Ȃ�
	}

	// ��������₪���ꂼ��̕ӂɐ����ȃx�N�g��
	XMVECTOR vS;
	for (int cntI = 0; cntI < 3; ++cntI)
	{
		for (int cntJ = 0; cntJ < 6; ++cntJ)
		{
			// �������������߂�
			vS = XMVector3Normalize(XMVector3Cross(vN[cntI], vN[cntJ]));

			// OBB�̉e�i�̔���)�̍��v
			fL = 0.0f;
			for (int cntK = 0; cntK < 6; ++cntK)
			{
				XMStoreFloat(&f, XMVector3Dot(vS, vL[cntK]));
				fL += fabsf(f);
			}

			// ���S���W�Ԃ̋����Ɣ�r
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD)
				return false;	// �������Ă��Ȃ�
		}
	}
	return true;
}

//==========================================================
// Collision���m�̓����蔻��
// �R���W�����׽���������ɂƂ��āA�����蔻����s��
//==========================================================
bool CBoxCollision::HitCheckPtr(CCollision* other)
{
	// null�m�F
	if (!other)
		return false;
	// ������̎擾
	CGameObject* otherOwner = other->GetOwner();

	// size����
	if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetSize(GetSize()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}

	// size�擾
	Vector3 size;
	// �h���N���X�ւ̃L���X�g
	if (CBoxCollision* com = dynamic_cast<CBoxCollision*>(other); com)
	{
		size = com->GetSize();
	}
	else if (CSphereCollision* com = dynamic_cast<CSphereCollision*>(other); com)
	{
		size = com->GetRadius();
	}
	else // �����ɗ��邱�Ƃ͑z�肵�Ă��Ȃ�
	{
		return false;
	}

	// �����蔻��
	if (m_bOBBMode)
	{
		if (!CollisionOBB(other->Transform(), other->GetCenter(), size))
			return false;
	}
	else
	{
		if (!CollisionAABB(other->Transform()->GetPos(), size))
			return false;
	}

	// �g���K�[��OFF�Ȃ牟���o��
	if (!IsTrigger())
	{
		PosAdjustment(other->Transform()->GetPos(), size));
		other->HitResponse(this);
	}
	this->HitResponse(other);

	return true;
}

//==========================================================
// �����o��
// TODO:���S�ł͂Ȃ��B�������������A�K�؂�size�ɂ�鉟���o���ł͂Ȃ�
//==========================================================
void CBoxCollision::PosAdjustment(Vector3 pos, Vector3 size)
{
	Vector3 checkPos = Transform()->GetPos();
	Vector3 oldPos = Transform()->GetOldPos();
	
	//---  �����o��
	// 2�_�ԂƂQ���a�̍�
	Vector3 distance = Transform()->GetPos() - pos;
	float len = (GetSize().GetLargeValue()*2 + size.GetLargeValue()*2) - distance.Length();
	// �����o������
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// �����o��
	Transform()->SetPos(Transform()->GetPos() + vec);
}


#ifdef BUILD_MODE

void CBoxCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	// 3�������W
	if (ImGui::DragFloat3("box size", (float*)&m_vSize))
	{
		m_pDebugBox->Init(m_vSize);
	}
	ImGui::Checkbox(u8"OBB", (bool*)&m_bOBBMode);

	if (ImGui::Button("box resize"))
	{
		if (auto model = GetComponent<CModelRenderer>(); model)
			m_vSize = model->GetModel().lock()->GetBBox();
		else
			m_vSize = Transform()->GetScale();
	}

}

// �Ƃ肠���������ɉ�����
void CBoxCollision::Update()
{
	if (!this->IsActive())
		return;

	//--- �f�o�b�O�\��
	XMFLOAT4X4 mW;
	if (m_bOBBMode)
	{
		// ���s�ړ��}�g���b�N�X����
		Vector3 center = GetCenter();
		XMMATRIX mMove = XMMatrixTranslation(center.x, center.y, center.z);
		// ���f��
		XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());
		mWorld = XMMatrixMultiply(mMove, mWorld);
		// ���E�{�b�N�X�\��
		XMStoreFloat4x4(&mW, mWorld);
		m_pDebugBox->SetWorld(&mW);
	}
	else
	{
		XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
		XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

		vCenter = XMVector3TransformCoord(vCenter, mWorld);
		mWorld = XMMatrixTranslationFromVector(vCenter);
		XMStoreFloat4x4(&mW, mWorld);

		m_pDebugBox->SetWorld(&mW);
	}

	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetInstanchingMesh(
		std::string(std::to_string(m_pDebugBox->GetIndexNum()) + std::to_string(m_pDebugBox->GetMaterial()->GetFloat())),
		mW,
		m_pDebugBox.get()
	);

}

#endif // BUILD_MODE