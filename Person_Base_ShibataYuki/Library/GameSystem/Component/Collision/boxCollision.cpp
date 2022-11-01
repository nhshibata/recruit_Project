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
#include <ImGui/imgui.h>

using namespace MySpace::Game;

//
CBoxCollision::CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size)
	: CCollision(owner),m_vSize(size)
{

}
CBoxCollision::~CBoxCollision()
{

}
// <Summary> 
// �����蔻��v�Z�֐�
// </Summary>
bool CBoxCollision::Box(Vector3 Apos, Vector3 Asize, Vector3 Bpos, Vector3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}
// �����蔻��
bool CBoxCollision::CollisionAABB(Vector3 pos, Vector3 size)
{
	if (Box(m_pTransform.lock()->GetPos(), m_vSize, pos, size))
	{
		return true;
	}
	return false;
}
// <Summary> 
// 
// �R���W�����׽���������ɂƂ��āA�����蔻����s��
// </Summary>
bool CBoxCollision::HitCheckPtr(CCollision* col)
{
	// ������̎擾
	CGameObject* owner = col->GetOwner();
	// null�m�F
	if (!col)
		return false;

	if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetSize(GetSize()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}

	Vector3 size;
	// �h���N���X�ւ̃L���X�g
	if (CBoxCollision* com = dynamic_cast<CBoxCollision*>(col); com)
	{
		size = com->GetSize();
	}
	else if (CSphereCollision* com = dynamic_cast<CSphereCollision*>(col); com)
	{
		size = com->GetRadius();
	}
	else // �����ɗ��邱�Ƃ͑z�肵�Ă��Ȃ�
	{
		return false;
	}

	// �����蔻��
	if (CollisionAABB(owner->GetTransform()->GetPos(), size))
	{
		// �g���K�[��OFF�Ȃ�ʒu�𒲐�
		if (!IsTrigger())
		{
			Transform()->SetPos(PosAdjustment(owner->GetTransform()->GetPos(), size));
		}
		HitResponse(col);
		return true;
	}

	return false;
}
Vector3 CBoxCollision::PosAdjustment(Vector3 pos, Vector3 size)
{
	Vector3 checkPos = m_pTransform.lock()->GetPos();
	Vector3 oldPos = m_pTransform.lock()->GetOldPos();
	Vector3 dist = Vector3::check(checkPos, oldPos);
	
	// �����Ȃ�
	if (dist == dist.zero())
		return checkPos;

	// �����Ă���
	if (dist.x == 1) 
	{
		// x�̊m�F
		checkPos.x = oldPos.x + dist.x;
		if (!Box(checkPos, m_vSize, pos, size))
		{
			return checkPos;
		}
	}
	if (dist.y == 1)
	{
		checkPos.y = oldPos.y + dist.y;
		if (!Box(checkPos, m_vSize, pos, size))
		{
			return checkPos;
		}
	}
	if (dist.z == 1)
	{
		checkPos.z = oldPos.z - dist.z;
		// �����܂ł��ē������Ă�Ȃ�ߋ����W��n��
		if (Box(checkPos, m_vSize, pos, size))
		{
			return oldPos;
		}
	}
	return checkPos;
}

#ifdef BUILD_MODE

void CBoxCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	// 3�������W
	ImGui::Checkbox(u8"�g���K�[", (bool*)&m_bIsTrigger);
	ImGui::InputFloat3(u8"�T�C�Y", (float*)&m_vSize);
	ImGui::Text(u8"StayNum %d", (int)m_pOldStayList.size());
}
#endif // BUILD_MODE