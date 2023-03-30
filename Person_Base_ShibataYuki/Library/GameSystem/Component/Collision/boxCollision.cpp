//=========================================================
// [boxCollision.h]
//---------------------------------------------------------
//�쐬:2022/05/02
//---------------------------------------------------------
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <CoreSystem/Time/fps.h>

#if BUILD_MODE

#include <DebugSystem/imGuiPackage.h>
#include <GraphicsSystem/Render/box.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>

#endif // BUILD_MODE

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CBoxCollision::CBoxCollision()
{

#if BUILD_MODE
	m_pDebugBox = std::make_shared<CBox>();
	m_pDebugBox->Init(Vector3(1,1,1));
	m_pDebugBox->SetDiffuse(Vector4(0, 1, 0, 0.5f));
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugBox->GetIndexNum()) + std::to_string(m_pDebugBox->GetMaterial()->GetFloat())),
		m_pDebugBox.get()
	);
#endif // BUILD_MODE

}
//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CBoxCollision::CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size)
	: CCollision(owner), m_vSize(size), m_bOBBMode(true)
{

#if BUILD_MODE
	m_pDebugBox = std::make_shared<CBox>();
	m_pDebugBox->Init(size);
	m_pDebugBox->SetDiffuse(Vector4(0, 1, 0, 0.5f));
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugBox->GetIndexNum()) + std::to_string(m_pDebugBox->GetMaterial()->GetFloat())),
		m_pDebugBox.get()
	);
#endif // BUILD_MODE

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBoxCollision::~CBoxCollision()
{
#if BUILD_MODE
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->ReleaseDebugMesh(m_pDebugBox.get());
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

	// size����
	/*if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetSize(GetSize()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}*/

	Vector3 size;
	// �h���N���X�ւ̃L���X�g
	if (CBoxCollision* com = dynamic_cast<CBoxCollision*>(other); com)
	{
		size = com->GetSize();
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
	}
	else if (CSphereCollision* com = dynamic_cast<CSphereCollision*>(other); com)
	{
		size = com->GetRadius();
		if (!this->IsCollidedWithBox(com))
		{
			return false;
		}
	}
	else // �����ɗ��邱�Ƃ͑z�肵�Ă��Ȃ�
	{
		return false;
	}

	// �g���K�[��OFF�Ȃ牟���o��
	if (!IsTrigger() && !other->IsTrigger())
	{
		size = other->Transform()->GetScale() * (size * 2);
		/*XMFLOAT4X4 mW1 = Transform()->GetWorldMatrix();
		XMFLOAT4X4 invWorld;
		XMMATRIX worldMat = XMLoadFloat4x4(&mW1);
		XMVECTOR det;
		worldMat = XMMatrixInverse(&det, worldMat);
		auto vSize = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(size.x, size.y, size.z, 1.0f), worldMat);
		DirectX::XMStoreFloat3(&size, vSize);*/

		PosAdjustment(other, other->Transform()->GetPos(), size);
		other->HitResponse(this);
	}
	this->HitResponse(other);

	return true;
}

//==========================================================
// �����o��
// TODO:���S�ł͂Ȃ��B�������������A�K�؂�size�ɂ�鉟���o���ł͂Ȃ�
//==========================================================
void CBoxCollision::PosAdjustment(CCollision* otherCol, Vector3 otherPos, Vector3 otherSize)
{
#if 1
	const Vector3 size = Transform()->GetScale() * (m_vSize * 2);
	const Vector3 currentPos = Transform()->GetPos();
	// ����
	float overlapDistance = CalculateOverlap(currentPos, otherPos, size, otherSize);
	Vector3 moveVec = CalculateMoveVector(currentPos, otherPos, overlapDistance);
	// �V�������W
	Vector3 newPos = currentPos;
	Vector3 newOtherPos = otherPos;
	// �ړ����Ă��邩�t���O
	bool sleep = false;
	bool otherSleep = false;
	{
		auto oldPos = Transform()->GetOldPos();
		sleep = currentPos != oldPos;
		oldPos = otherCol->Transform()->GetOldPos();
		otherSleep = otherPos != oldPos;
	}
	if (!sleep && !otherSleep)
		return;

	if(sleep)
		newPos += moveVec;
	if(otherSleep)
		newOtherPos += moveVec;

	int loopCount = 0;
	const int maxLoopCount = 10;
	const float minOverlapDistance = 0.01f;
	while (loopCount < maxLoopCount)
	{
		if (CBoxCollision::Box(newPos, size, newOtherPos, otherSize))
		{
			overlapDistance = CalculateOverlap(newPos, newOtherPos, size, otherSize);
			moveVec = CalculateMoveVector(newPos, newOtherPos, overlapDistance);
			if (sleep)
				newPos += moveVec;
			if (otherSleep)
				newOtherPos += moveVec;				

			// �߂荞�݋��������l�ȉ��ɂȂ����ꍇ�̓��[�v���I��
			if (overlapDistance < minOverlapDistance)
				break;

			loopCount++;
		}
		else
			break;
	}

	Transform()->SetPos(newPos);
	otherCol->Transform()->SetPos(newOtherPos);
	

#elif 2
	// ���݈ʒu�ƈȑO�̈ʒu���擾
	Vector3 currentPos = Transform()->GetPos();
	const Vector3 oldPos = Transform()->GetOldPos();
	
	// 2�̋�`�̋������v�Z
	Vector3 distance = otherPos - currentPos;
	const float totalRadius = (GetSize().GetLargeValue() + otherSize.GetLargeValue());
	const float distanceLength = distance.Length();

	// ��`���m���d�Ȃ��Ă���ꍇ
	if (distanceLength < totalRadius)
	{
		// �����߂�����
		if (distanceLength > 0.0f)
		{
			Vector3 direction = distance.Normalize();
			Vector3 displacement = direction * (totalRadius - distanceLength);
			Vector3 newPos = currentPos - displacement;

			Transform()->SetPos(newPos);

			if (CBoxCollision::Box(newPos, this->m_vSize, otherPos, otherSize))
			{
				Transform()->SetPos(oldPos);
			}
		}
		else
		{
			// ��`���m���d�Ȃ��Ă��邪�A�ʒu�������ꍇ
			// �ړ�����~���Ă��܂����߁A��x�O�̈ʒu�ɖ߂�
			Transform()->SetPos(oldPos);
		}
	}
#elif 3
	// ���݈ʒu�ƈȑO�̈ʒu���擾
	Vector3 currentPos = Transform()->GetPos();
	const Vector3 oldPos = Transform()->GetOldPos();

	// ���S�_����̋������v�Z
	Vector3 distance = otherPos - currentPos;
	float distanceX = fabsf(distance.x);
	float distanceY = fabsf(distance.y);
	float distanceZ = fabsf(distance.z);

	// �e��`�̃T�C�Y�̔������v�Z
	float myHalfX = m_vSize.x / 2.0f;
	float myHalfY = m_vSize.y / 2.0f;
	float myHalfZ = m_vSize.z / 2.0f;
	float otherHalfX = otherSize.x / 2.0f;
	float otherHalfY = otherSize.y / 2.0f;
	float otherHalfZ = otherSize.z / 2.0f;

	// ��`���m���d�Ȃ��Ă���ꍇ
	if (distanceX < myHalfX + otherHalfX &&
		distanceY < myHalfY + otherHalfY &&
		distanceZ < myHalfZ + otherHalfZ)
	{
		// �����߂�����
		float xOverlap = myHalfX + otherHalfX - distanceX;
		float yOverlap = myHalfY + otherHalfY - distanceY;
		float zOverlap = myHalfZ + otherHalfZ - distanceZ;

		if (xOverlap < yOverlap && xOverlap < zOverlap)
		{
			// x�������ɉ����߂�
			if (distance.x < 0)
			{
				currentPos.x -= xOverlap;
			}
			else
			{
				currentPos.x += xOverlap;
			}
		}
		else if (yOverlap < zOverlap)
		{
			// y�������ɉ����߂�
			if (distance.y < 0)
			{
				currentPos.y -= yOverlap;
			}
			else
			{
				currentPos.y += yOverlap;
			}
		}
		else
		{
			// z�������ɉ����߂�
			if (distance.z < 0)
			{
				currentPos.z -= zOverlap;
			}
			else
			{
				currentPos.z += zOverlap;
			}
		}

		// ���W���X�V
		Transform()->SetPos(currentPos);
	}
#else

	Vector3 checkPos = Transform()->GetPos();
	Vector3 oldPos = Transform()->GetOldPos();

	//---  �����o��
	// 2�_�ԂƂQ���a�̍�
	Vector3 distance = Transform()->GetPos() - otherPos;
	float len = (GetSize().GetLargeValue() * 2 + otherSize.GetLargeValue() * 2) - distance.Length();
	// �����o������
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// �����o��
	Transform()->SetPos(Transform()->GetPos() + vec);

#endif // 1

}

bool CBoxCollision::IsCollidedWithBox(CSphereCollision* sphere)
{
	// ���Ƌ�`�̒��S�_�̋���
	Vector3 distance = sphere->Transform()->GetPos() - this->Transform()->GetPos();
	float distLen = distance.Length();

	// ��`�̑Ίp���̒���
	float diagonalLen = sqrt(pow(this->GetSize().x, 2) + pow(this->GetSize().y, 2) + pow(this->GetSize().z, 2)) / 2;

	// ���Ƌ�`�̒��S�_�̋������A���̔��a�Ƌ�`�̑Ίp���̒����̔��������������ꍇ�A������Ƃ݂Ȃ�
	if (distLen < sphere->GetRadius() + diagonalLen)
	{
		return true;
	}

	return false;
}

//==========================================================
// 2�̃{�b�N�X���߂荞��ł��鋗�����v�Z����֐�
//==========================================================
float CBoxCollision::CalculateOverlap(
	const Vector3& currentPos, const Vector3& otherPos, 
	const Vector3& size1, const Vector3& size2)
{
	// 2�̋�`�̒��S�Ԃ̋������v�Z
	Vector3 bPos = otherPos;
	Vector3 centerOffset =  bPos - currentPos;

	// 2�̋�`�̕��̍��v
	float totalWidth = size1.x + size2.x;

	// 2�̋�`�̍����̍��v
	float totalHeight = size1.y + size2.y;

	// 2�̋�`�̉��s���̍��v
	float totalDepth = size1.z + size2.z;

	// 2�̋�`�̕��A�����A���s���̂����A�ŏ��̂��̂��擾
	float minDimension = std::min(std::min(totalWidth, totalHeight), totalDepth);
	
	float length = centerOffset.Length();

	// 2�̋�`�̒��S�Ԃ̋������ŏ��̋�`�̑傫����菬�����ꍇ�A�߂荞��ł��鋗�����v�Z
	if (length < minDimension / 2)
	{
		return minDimension / 2 - length;
	}

	return 0.0f;
}

//==========================================================
// �߂荞�񂾋�������ړ��x�N�g�����v�Z����֐�
//==========================================================
Vector3 CBoxCollision::CalculateMoveVector(const Vector3& currentPos, const Vector3& otherPos, const float& overlap)
{
	// 2�̃{�b�N�X�̈ʒu�̍������擾
	/*Vector3 aPos = currentPos;
	Vector3 direction = aPos - otherPos;*/
	Vector3 aPos = otherPos;
	Vector3 direction = aPos - currentPos;

	// �m�[�}���C�Y���Ă߂荞�ݕ��̋������ړ�����x�N�g�����擾
	direction = direction.Normalize();
	direction *= overlap;

	return direction;
}



#ifdef BUILD_MODE

void CBoxCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	ImGui::Separator();

	Debug::SetTextAndAligned("OBB");
	ImGui::Checkbox("##OBB", (bool*)&m_bOBBMode);

	Debug::SetTextAndAligned("box size");
	// 3�������W
	if (ImGui::DragFloat3("##boxsize", (float*)&m_vSize))
	{
		m_pDebugBox->Init(m_vSize);
	}

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
	//--- �f�o�b�O�\��
	XMFLOAT4X4 mW;
	if (m_bOBBMode)
	{
		// ���s�ړ��}�g���b�N�X����
		Vector3 center = GetCenter();
		// 0.001��Z�t�@�C�e�B���O�h�~
		auto renderSize = Transform()->GetScale() * (m_vSize / 2) * 1.001f;
		XMMATRIX mMove = XMMatrixTranslation(center.x, center.y, center.z);
		Matrix4x4 mtx = Matrix4x4::CalcWorldMatrix(Transform()->GetPos(), Transform()->GetRot(), renderSize);
		
		// ���f��
		XMMATRIX mWorld = XMLoadFloat4x4(&mtx);
		mWorld = XMMatrixMultiply(mMove, mWorld);
		// ���E�{�b�N�X�\��
		XMStoreFloat4x4(&mW, mWorld);
		m_pDebugBox->SetWorld(&mW);
	}
	else
	{
		XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
		// 0.001��Z�t�@�C�e�B���O�h�~
		auto renderSize = Transform()->GetScale() * (m_vSize / 2) * 1.001f;
		Matrix4x4 mtx = Matrix4x4::CalcWorldMatrix(Transform()->GetPos(), Transform()->GetRot(), renderSize);
		XMMATRIX mWorld = XMLoadFloat4x4(&mtx);

		vCenter = XMVector3TransformCoord(vCenter, mWorld);
		mWorld = XMMatrixTranslationFromVector(vCenter);
		XMStoreFloat4x4(&mW, mWorld);

		m_pDebugBox->SetWorld(&mW);
	}

}

#endif // BUILD_MODE