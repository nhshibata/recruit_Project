//=========================================================
// [resultUI.cpp] 
// �쐬:2022/08/18
//---------------------------------------------------------
//=========================================================

#include <resultUI.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <UIAnime.h>

//
void CResultUI::Awake()
{
	GetOwner()->SetName("ResultSceneUI");

	// �t�F�[�h�p
	auto obj = CGameObject::CreateObject().lock();
	auto com = obj->AddComponent<CPolygonRenderer>();
	obj->SetLayer(CLayer::E_Layer::BG);
	com->GetRectTransform()->SetSize(CScreen::GetSize());
	
	// �e�L�X�g
	obj = CGameObject::CreateObject().lock();
	m_pResultText = obj->AddComponent<CTextRenderer>();
	m_pResultText.lock()->SetTextWString(L"Result");
	m_pResultText.lock()->SetColor({0,0,0,1});
	m_pResultText.lock()->GetRectTransform()->SetPos(0, 0);
	m_pResultText.lock()->GetRectTransform()->SetSize(CScreen::GetWidth() / 3, CScreen::GetHeight() / 3);
	
	obj = CGameObject::CreateObject().lock();
	m_pButtonText = obj->AddComponent<CTextRenderer>();
	m_pButtonText.lock()->SetTextWString(L"Please Space");
	m_pButtonText.lock()->SetColor({1,1,1,1});
	m_pButtonText.lock()->GetRectTransform()->SetPos(0, -200);

	// UIAnime
	obj = CGameObject::CreateObject().lock();
	obj->AddComponent<CUIAnimeRot>();
}
void CResultUI::Init()
{
	// ���S�ɑ����{���\��
	auto obj = CGameObject::CreateObject().lock();
	m_pLengthText = obj->AddComponent<CTextRenderer>();
	int num = (m_nDispNum - 2) * 2; // ���̐��̂��߁A�����͊܂߂Ȃ�
	if (num == 50)
	{
		m_pLengthText.lock()->SetTextWString(L"�S��");
	}
	else
	{
		m_pLengthText.lock()->SetTextWString(std::to_wstring(num));		
	}
	m_pLengthText.lock()->SetColor({ 1,1,1,1 });
	m_pLengthText.lock()->GetRectTransform()->SetPos(0, 0);
}
void CResultUI::Update()
{
	if (m_pButtonText.lock())
	{
		auto color = m_pButtonText.lock()->GetColor();
		m_pButtonText.lock()->SetColor({ color.a-CFps::Get().DeltaTime(), 0 ,color.b-CFps::Get().DeltaTime() ,1 });
		if(color.a < 0)
			m_pButtonText.lock()->SetColor({ 1,1,1,1 });
	}
}