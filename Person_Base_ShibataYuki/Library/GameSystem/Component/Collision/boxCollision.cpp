//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//
//=========================================================

//--- インクルード部
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
// 当たり判定計算関数
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
// 当たり判定
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
// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
// </Summary>
bool CBoxCollision::HitCheckPtr(CCollision* col)
{
	// 持ち主の取得
	CGameObject* owner = col->GetOwner();
	// null確認
	if (!col)
		return false;

	if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetSize(GetSize()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}

	Vector3 size;
	// 派生クラスへのキャスト
	if (CBoxCollision* com = dynamic_cast<CBoxCollision*>(col); com)
	{
		size = com->GetSize();
	}
	else if (CSphereCollision* com = dynamic_cast<CSphereCollision*>(col); com)
	{
		size = com->GetRadius();
	}
	else // ここに来ることは想定していない
	{
		return false;
	}

	// 当たり判定
	if (CollisionAABB(owner->GetTransform()->GetPos(), size))
	{
		// トリガーがOFFなら位置を調整
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
	
	// 差がない
	if (dist == dist.zero())
		return checkPos;

	// 動いている
	if (dist.x == 1) 
	{
		// xの確認
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
		// ここまできて当たってるなら過去座標を渡す
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

	// 3次元座標
	ImGui::Checkbox(u8"トリガー", (bool*)&m_bIsTrigger);
	ImGui::InputFloat3(u8"サイズ", (float*)&m_vSize);
	ImGui::Text(u8"StayNum %d", (int)m_pOldStayList.size());
}
#endif // BUILD_MODE