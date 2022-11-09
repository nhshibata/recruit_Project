//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================

//--- インクルード部
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
// 球の当たり判定
bool CSphereCollision::CollisionSphere(Vector3 pos, float radius)
{
	if (Sphere(Transform()->GetPos(), m_fRadius, pos, radius))
	{
		return true;
	}
	return false;
}		
// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
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

	// ｺﾝﾎﾟｰﾈﾝﾄの確認
	if (CSphereCollision* com = other->GetComponent<CSphereCollision>(); com)
	{
		// 球の判定
		if (CollisionSphere(other->Transform()->GetPos(), com->GetRadius()))
		{
			// 押し出し
			if (!trigger)
			{
				// TODO: 確認
				//Transform()->SetPos(PosAdjustment(other->Transform()->GetPos(), com->GetRadius()));
				PushObject(com);
				// 押し出しを行うと相手は判定されないため、ここで行う
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
			// 押し出し
			if (!trigger)
			{
				// TODO: 確認
				//Transform()->SetPos(otherBox->PosAdjustment(other->Transform()->GetPos(), size));
				PushObject(com);
				// 押し出しを行うと相手は判定されないため、ここで行う
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

	// 差がない
	if (dist == dist.zero())
		return checkPos;
	
	// 動いている
	if (dist.x == 1)
	{
		// xの確認
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
		// ここまできて当たってるなら過去座標を渡す
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

	// 3次元座標
	//ImGui::Checkbox(u8"状態", &IsActive());
	if (ImGui::DragFloat3(u8"サイズ", &m_fRadius))
	{
		m_pDebugSphere->Init(16, 8, m_fRadius);
	}

	m_pDebugSphere->SetWorld(&Transform()->GetWorldMatrix());
	m_pDebugSphere->Draw();
}
#endif // BUILD_MODE