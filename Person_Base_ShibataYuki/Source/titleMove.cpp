//========================================================
// [titleMove.h]
//--------------------------------------------------------
// �쐬:2023/01/10 
//--------------------------------------------------------
// ���ړ��̂�
//========================================================

//--- �C���N���[�h��
#include <titleMove.h>

using namespace Spell;


CTitleMove::CTitleMove()
{

}

CTitleMove::CTitleMove(CGameObject::Ptr owner)
	:CComponent(owner)
{

}

CTitleMove::~CTitleMove()
{

}

void CTitleMove::Awake()
{

}

void CTitleMove::Init()
{
	auto com = GetOwner()->GetComponent<CPolygonRenderer>();
	if (!com.lock())
	{
		m_pPolygon = GetOwner()->AddComponent<CPolygonRenderer>();
		return;
	}
	m_pPolygon = com.lock();
}

void CTitleMove::Update()
{
	// �������܂��Ă��Ȃ�
	if (m_aCurveList.size() == 0)
	{
		Vector2 vec1, vec2, endPos;
		int num = rand() % 200 + 100;
		auto pos = m_pPolygon.lock()->GetRectTransform()->GetPos();
		float dir = pos.x;
		endPos = pos;
		
		// �E
		if (dir < 0)
		{
			vec1 = Vector2(1, 1);
			vec2 = Vector2(1, -1);
			endPos.x = CScreen::GetHalfWidth();
		}
		// ��
		else 
		{
			vec1 = Vector2(1, -1);
			vec2 = Vector2(1, 1);
			endPos.x = -CScreen::GetHalfWidth();
		}

		m_aCurveList = ST_MyMath::MakeHermiteCurve(pos, vec1, endPos, vec2, num);
		return;
	}
	else
	{
		// �擪���珇�Ɉړ�
		m_pPolygon.lock()->GetRectTransform()->SetPos(m_aCurveList.front());
		m_aCurveList.erase(m_aCurveList.begin());
	}

}

void CTitleMove::SetStartPos(const Vector2 pos)
{
	m_vStartPos = pos; 
	m_pPolygon.lock()->GetRectTransform()->SetPos(pos);
}