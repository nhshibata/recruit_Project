//=========================================================
// [gameUI.cpp] 
// 作成:2022/08/18
//---------------------------------------------------------
//=========================================================

/*
	描画するもの
	1.長さ
	2.胴体の数
	3.経験値
	4.速度
*/

//--- インクルード部
#include <gameUI.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Component/Renderer/textRenderer.h>

namespace
{
	//const Vector2 GAUGE_SIZE = CScreen::GetWidth()*0.9f, CScreen::GetHeight()*0.9f;
}

void CGameUI::Awake()
{
	// フェード用
	GetOwner()->SetName("GameSceneUI");

	auto obj = CGameObject::CreateObject().lock();
	float sizeX = CScreen::GetWidth() / 11;
	float posX_L = -CScreen::GetWidth()/2 + 100;
	float posX_R = posX_L + sizeX * 9;
	float posY = CScreen::GetHeight() / 2 - sizeX;

	// ゲージ
	obj = CGameObject::CreateObject().lock();
	m_pGauge = obj->AddComponent<CPolygonRenderer>();
	m_pGauge.lock()->GetRectTransform()->SetSize(CScreen::GetWidth()*0.9f, CScreen::GetHeight()*0.9f);

	// Life
	auto render = obj->AddComponent<CPolygonRenderer>();
	render->SetImageName(TEXTURE_NAME(mukade_h_UI.png));
	render->GetRectTransform()->SetAngle(-90);
	render->GetRectTransform()->SetPos(posX_L, posY);
	render->GetRectTransform()->SetSize({ sizeX ,sizeX });
	m_pLength.push_back(render);
	
	for (int cnt = 0; cnt < 8; ++cnt)
	{
		posX_L += sizeX;
		obj = CGameObject::CreateObject().lock();
		render = obj->AddComponent<CPolygonRenderer>();
		render->SetImageName(TEXTURE_NAME(mukade_UI.png));
		render->GetRectTransform()->SetAngle(90);
		render->GetRectTransform()->SetPos(posX_L, posY);
		render->GetRectTransform()->SetSize({ sizeX ,sizeX });
		m_pLength.push_back(render);
	}

	obj = CGameObject::CreateObject().lock();
	render = obj->AddComponent<CPolygonRenderer>();
	render->SetImageName(TEXTURE_NAME(mukade_h_UI.png));
	render->GetRectTransform()->SetAngle(90);
	render->GetRectTransform()->SetPos(posX_R, posY);
	render->GetRectTransform()->SetSize({ sizeX ,sizeX });
	m_pLength.push_back(render);


	//Transform()->AddChild(obj);
}
void CGameUI::Init()
{
	m_pHead = CGameObject::FindGameObject("PlayerManager").lock()->GetComponent<CCentipede>();

	for (int cnt = 0; cnt < static_cast<int>(m_pLength.size()); ++cnt)
	{
		//if(cnt == static_cast<int>(m_pLength.size()) -1)
		if(cnt == 0)
			m_pLength[cnt].lock()->GetRectTransform()->SetAngle(90);
		else
			m_pLength[cnt].lock()->GetRectTransform()->SetAngle(-90);
	}
}
void CGameUI::Update()
{
	int life = static_cast<int>(m_pHead.lock()->GetLife() / m_pHead.lock()->GetMaxLife());
	int num = static_cast<int>(m_pLength.size()) - life;
	if (m_nPrevLife != num)
	{
		for (int cnt = 0; cnt < num; ++cnt)
		{
			if (!m_pLength[cnt].lock())
				continue;
			m_pLength[cnt].lock()->SetActive(false);
		}
		m_nPrevLife = num;
	}
	
	m_pHead.lock()->GetGrowGauge();
	auto size = m_pGauge.lock()->GetRectTransform()->GetSize() / Vector2(CScreen::GetWidth()*0.9f, CScreen::GetHeight() * 0.9f);
	


}
