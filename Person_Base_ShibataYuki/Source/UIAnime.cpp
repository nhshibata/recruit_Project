//=========================================================
// [UIAnime.cpp]
// �쐬:2022/09/05
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <UIAnime.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <CoreSystem/Input/input.h>

CUIAnime::CUIAnime(std::shared_ptr<CGameObject> owner)
	:CComponent(owner)
{
}
void CUIAnime::Awake()
{
	GetOwner()->SetName("UIAnime");

	Vector2 pos;
	m_fPolygonDir = static_cast<float>(rand() % 360 - 180);
	m_pMyRect = AddComponent<CRectTransform>();

	m_fSpeed = 2.0f;
	// obj����
	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		auto obj = CGameObject::CreateObject().lock();
		auto render = obj->AddComponent<CPolygonRenderer>();			// �|���S��
		if (cnt == 0)													// �摜�ݒ�
			render->SetImageName(TEXTURE_NAME(mukade_h_UI.png));
		else if(cnt == m_nPolygonNum-1)
			render->SetImageName(TEXTURE_NAME(mukade_h_UI.png));
		else
			render->SetImageName(TEXTURE_NAME(mukade_UI.png));
		render->SetLayer(static_cast<int>(CLayer::E_Layer::UI) - 1);
		m_pRectList.push_back(obj->GetComponent<CRectTransform>());
		m_pRectList.back().lock()->SetSize(100, 100);

		// ���S���炠����x���ꂽ�ꏊ��
		pos.x += cosf(DirectX::XMConvertToRadians(m_fPolygonDir)) * m_fSpeed * 2;
		pos.y += sinf(DirectX::XMConvertToRadians(m_fPolygonDir)) * m_fSpeed * 2;
		m_vParamList.push_back(STParam(pos, (m_fPolygonDir - 90.0f)));
	}
	m_pMyRect.lock()->SetPos(pos);
}
void CUIAnime::Update()
{
	Vector2 pos = m_pMyRect.lock()->GetPos();

	// ���W�X�V
	if (auto newpos = CScreen::ScreenReverse(pos); newpos != pos)
	{
		m_fPolygonDir = m_fPolygonDir + static_cast<float>(rand() % 90 - 90);
	}
	pos.x += cosf(DirectX::XMConvertToRadians(m_fPolygonDir)) * m_fSpeed;
	pos.y += sinf(DirectX::XMConvertToRadians(m_fPolygonDir)) * m_fSpeed;
	m_pMyRect.lock()->SetPos(pos);

	++m_nUpdateCnt;
	if (m_nUpdateCnt < 25)
		return;

	// ��̍X�V
	m_nUpdateCnt = 0;
	m_vParamList.push_back(STParam(pos, (m_fPolygonDir - 90.0f)));
	m_vParamList.erase(m_vParamList.begin());

	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		m_pRectList[cnt].lock()->SetPos(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].vPos);
		if(cnt == m_nPolygonNum-1)
			m_pRectList[cnt].lock()->SetAngle(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].fAngle + 180.0f);
		else
			m_pRectList[cnt].lock()->SetAngle(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].fAngle);
	}
}
//
void CUIAnimeRot::Awake()
{
	CUIAnime::Awake();
	for (int cnt = 0; cnt < m_nPolygonNum * 5; ++cnt)
	{
		Update();
	}
}
void CUIAnimeRot::Update()
{
	Vector2 pos = m_pMyRect.lock()->GetPos();

	// ���W�X�V
	++m_fPolygonDir;
	if (m_fPolygonDir >= 180.0f)
		m_fPolygonDir -= 360.0f;

	pos.x = cosf(DirectX::XMConvertToRadians(m_fPolygonDir)) * CScreen::GetWidth() / 3;
	pos.y = sinf(DirectX::XMConvertToRadians(m_fPolygonDir)) * CScreen::GetWidth() / 3;
	m_pMyRect.lock()->SetPos(pos);

	++m_nUpdateCnt;
	if (m_nUpdateCnt < 5)
		return;

	// ��̍X�V
	m_nUpdateCnt = 0;
	m_vParamList.push_back(STParam(pos, (m_fPolygonDir)));
	m_vParamList.erase(m_vParamList.begin());

	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		m_pRectList[cnt].lock()->SetPos(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].vPos);
		if (cnt == m_nPolygonNum - 1)
			m_pRectList[cnt].lock()->SetAngle(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].fAngle + 180.0f);
		else
		m_pRectList[cnt].lock()->SetAngle(m_vParamList[static_cast<int>(m_vParamList.size()) - (cnt + 1)].fAngle);
	}
}
//
void CUIAnimeCurve::Awake()
{
	m_nPolygonNum = 20;
	CUIAnime::Awake();
	InitCurve();
	// ������
	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		Update();
	}
}
void CUIAnimeCurve::Update()
{
	// �J�E���g�̍X�V
	++m_nUpdateCnt;
	if (m_nUpdateCnt < 6)
		return;

	m_nUpdateCnt = 0;
	// index�ԍ��̍X�V
	++m_nCurveIdx;
	if ((size_t)m_nCurveIdx >= m_vParamList.size()-1)
	{
		m_nCurveIdx = 0;
		InitCurve();
		return;
	}
	// �p�����[�^�X�V
	CUIAnime::STParam param;		// ����ւ��p�ϐ�
	CUIAnime::STParam paramPrev;	// ����ւ��p�ϐ�
	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		if (cnt == 0)// �擪�ɂ̓J�[�u�̍��W��
		{
			param = { m_pRectList[cnt].lock()->GetPos(),m_pRectList[cnt].lock()->GetAngle() };

			m_pRectList[cnt].lock()->SetPos(m_vParamList[m_nCurveIdx].vPos);
			m_pRectList[cnt].lock()->SetAngle(m_vParamList[m_nCurveIdx].fAngle);
		}
		// �㑱�ɂ͎g���I��������W�������[
		else
		{
			paramPrev = { m_pRectList[cnt].lock()->GetPos(),m_pRectList[cnt].lock()->GetAngle() };
			m_pRectList[cnt].lock()->SetPos(param.vPos);
			m_pRectList[cnt].lock()->SetAngle(param.fAngle);
			param = paramPrev;
		}
	}
	m_pRectList[m_nPolygonNum - 1].lock()->SetAngle(m_pRectList[m_nPolygonNum - 1].lock()->GetAngle() + 180.0f);
}
void CUIAnimeCurve::InitCurve()
{
	// ���������W������
	Vector2 pos = m_vParamList[m_nPolygonNum-1].vPos;
	Vector2 newPos = pos;

	// ��ʓ����m�F���A���[�v�Ŋm��
	{
		int cnt = 0;
		float posx,posz;
		posx = posz = 0;
	do
	{
		newPos = pos;
		if (cnt > 30)
		{
			posx = static_cast<float>(rand() % 1200);
			posz = static_cast<float>(rand() % 1200);
		}
		else
		{
			posx = static_cast<float>(rand() % 600 + 200);
			posz = static_cast<float>(rand() % 600 + 200);
		}
		m_fPolygonDir = static_cast<float>(rand() % 360 - 180);
		newPos.x += cosf(DirectX::XMConvertToRadians(m_fPolygonDir)) * posx;
		newPos.y += sinf(DirectX::XMConvertToRadians(m_fPolygonDir)) * posz;
		++cnt;

	} while (CScreen::ScreenJudg(newPos));
	}
	// �J�[�u���v�Z
	Vector2 dir = (pos + Vector2(cosf(DirectX::XMConvertToRadians(m_fPolygonDir)), sinf(DirectX::XMConvertToRadians(m_fPolygonDir))));
	auto vec = ST_MyMath::MakeHermiteCurve(
		pos,
		(pos + Vector2(cosf(DirectX::XMConvertToRadians(m_fPolygonDir))*30, sinf(DirectX::XMConvertToRadians(m_fPolygonDir))*30)),
		newPos,
		(newPos + Vector2(cosf(DirectX::XMConvertToRadians(m_fPolygonDir))*30, sinf(DirectX::XMConvertToRadians(m_fPolygonDir))*30)),
		m_nPolygonNum
	);
	// ���W�Ɗp�x���i�[
	for (int cnt = 0; cnt < m_nPolygonNum; ++cnt)
	{
		m_vParamList[cnt].vPos = vec[cnt];
		if (cnt + 1 < static_cast<int>(vec.size()))
		{
			m_vParamList[cnt].fAngle = -XMConvertToDegrees(atan2f(vec[cnt + 1].x - vec[cnt].x, vec[cnt + 1].y - vec[cnt].y));
		}
	}
}