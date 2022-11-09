//=========================================================
//�쐬:2022/05/02
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>
#include <GraphicsSystem/Render/Sphere.h>

using namespace MySpace::Game;

//
CSphereCollision::CSphereCollision(std::shared_ptr<CGameObject> owner, float radius)
	:CCollision(owner),m_fRadius(radius)
{
#if BUILD_MODE
	m_pDebugSphere = std::make_shared<CSphere>();
	m_pDebugSphere->Init(16, 8, radius);
#endif //
}
//
CSphereCollision::~CSphereCollision()
{
#if BUILD_MODE
	m_pDebugSphere.reset();
#endif // BUILD_MODE
}
bool CSphereCollision::Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}
// ���̓����蔻��
bool CSphereCollision::CollisionSphere(Vector3 pos, float radius)
{
	if (Sphere(Transform()->GetPos(), m_fRadius, pos, radius))
	{
		return true;
	}
	return false;
}		
// �R���W�����׽���������ɂƂ��āA�����蔻����s��
bool CSphereCollision::HitCheckPtr(CCollision* other)
{
	bool trigger = other->IsTrigger() | IsTrigger();
#ifdef _DEBUG
	auto other_name = other->GetOwner()->GetName();
	auto other_flg = other->IsActive();
	auto name = GetOwner()->GetName();
	auto flg = IsActive();
#endif // _DEBUG
	if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetRadius(GetRadius()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}

	// ���߰��Ă̊m�F
	if (CSphereCollision* com = other->GetComponent<CSphereCollision>(); com)
	{
		// ���̔���
		if (CollisionSphere(other->Transform()->GetPos(), com->GetRadius()))
		{
			// �����o��
			if (!trigger)
			{
				// TODO: �m�F
				//Transform()->SetPos(PosAdjustment(other->Transform()->GetPos(), com->GetRadius()));
				PushObject(com);
				// �����o�����s���Ƒ���͔��肳��Ȃ����߁A�����ōs��
				HitResponse(this);
			}
			HitResponse(other);
			
			return true;
		}
	}
	else if (auto otherBox = other->GetComponent<CBoxCollision>(); otherBox)
	{
		Vector3 size = { this->GetRadius() ,this->GetRadius() ,this->GetRadius() };
		if (otherBox->Box(Transform()->GetPos(), size, other->Transform()->GetPos(), otherBox->GetSize()))
		{
			// �����o��
			if (!trigger)
			{
				// TODO: �m�F
				//Transform()->SetPos(otherBox->PosAdjustment(other->Transform()->GetPos(), size));
				PushObject(com);
				// �����o�����s���Ƒ���͔��肳��Ȃ����߁A�����ōs��
				HitResponse(this);
			}
			HitResponse(other);

			return true;
		}
	}

	return false;
}

Vector3 CSphereCollision::PosAdjustment(Vector3 otherPos, float size)
{
	Vector3 checkPos = Transform()->GetPos();
	Vector3 oldPos = Transform()->GetOldPos();
	Vector3 dist = Vector3::check(checkPos, oldPos);

	// �����Ȃ�
	if (dist == dist.zero())
		return checkPos;
	
	// �����Ă���
	if (dist.x == 1)
	{
		// x�̊m�F
		checkPos.x = oldPos.x + dist.x;
		if (!Sphere(checkPos, m_fRadius, otherPos, size))
		{
			return checkPos;
		}
	}
	if (dist.y == 1)
	{
		checkPos.y = oldPos.y + dist.y;
		if (!Sphere(checkPos, m_fRadius, otherPos, size))
		{
			return checkPos;
		}
	}
	if (dist.z == 1)
	{
		checkPos.z = oldPos.z - dist.z;
		// �����܂ł��ē������Ă�Ȃ�ߋ����W��n��
		if (Sphere(checkPos, m_fRadius, otherPos, size))
		{
			return oldPos;
		}
	}
	return checkPos;
}

#ifdef BUILD_MODE
void CSphereCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	// 3�������W
	//ImGui::Checkbox(u8"���", &IsActive());
	if (ImGui::DragFloat3(u8"�T�C�Y", &m_fRadius))
	{
		m_pDebugSphere->Init(16, 8, m_fRadius);
	}

	m_pDebugSphere->SetWorld(&Transform()->GetWorldMatrix());
	m_pDebugSphere->Draw();
}
#endif // BUILD_MODE