//=========================================================
// [timer.cpp]
// 作成:2022/09/08
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <timer.h>

void CTimer::Awake()
{
	// 認識用設定
	GetOwner()->SetName(CTimer::TAG_NAME);
	GetOwner()->GetTagPtr()->CreateTag(CTimer::TAG_NAME);
	
	m_pText = AddComponent<CTextRenderer>();
	float max = m_fTime;
	auto n1 = max / 100;
	max -= n1;
	auto n2 = max / 10;
	max -= n2;
	auto n3 = max / 1;
	max -= n3;
	auto n4 = max / 0.1f;

	m_pText.lock()->SetTextWString(
		std::to_wstring(n1) +
		std::to_wstring(n2) +
		std::to_wstring(n3) +
		L"." +
		std::to_wstring(n4)
	);
}
void CTimer::Update()
{
	m_fTime -= CFps::Get().DeltaTime();

	int max = static_cast<int>(m_fTime * 10);
	int n1 = max / 1000;
	max %= 1000;
	int n2 = max / 100;
	max %= 100;
	int n3 = max / 10;
	max %= 10;
	int n4 = max / 1;
	
	if (n1 == 0 && n2 == 0 && n3 == 0)
	{
		m_pText.lock()->SetTextWString(L"000.0");
		return;
	}

	m_pText.lock()->SetTextWString(
		std::to_wstring(n1) +
		std::to_wstring(n2) +
		std::to_wstring(n3) +
		L"." +
		std::to_wstring(n4)
	);

	if (n1 == 0)
	{
		if (n2 < 5)
		{
			m_pText.lock()->SetColor({ 0,1,1,1 });
			if (n2 < 3)
				m_pText.lock()->SetColor({ 1,0,0,1 });
		}
		else
			m_pText.lock()->SetColor({ 0,0,1,1 });
	}
	
}