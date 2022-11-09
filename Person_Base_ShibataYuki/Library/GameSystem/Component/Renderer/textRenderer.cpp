//=========================================================
// [textRenerer.cpp] 
// 作成: 2022/08/04
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/textRenderer.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Render/polygon.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/Util/stringConvert.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

		
CTextRenderer::CTextRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner), m_fOffset(0), m_Font(L"ＭＳ Ｐ明朝"), m_bVerticalWrit(false)
{

}
CTextRenderer::~CTextRenderer()
{

}
void CTextRenderer::Awake()
{
	GetOwner()->SetName("Text");
	m_Text = L"Text";

	GetOwner()->SetLayer(CLayer::E_Layer::UI);

	// コンポーネントの取得
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();

	// なければ追加
	if (!m_pRectTransform.lock()) {
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}
}
void CTextRenderer::Init()
{
	// サイズ取得
	m_uOldTextSize = m_Text.size();
	
	// 文字列からﾃｸｽﾁｬ取得
	m_pTexList = CFontTexture::Get().GetString(m_Text, m_Font);

	// 描画依頼
	CRenderer::Init();
}
void CTextRenderer::Update()
{
	// サイズ比較
	//if (m_uOldTextSize != m_Text.size())
	//{
	//	m_pTexList = CFontTexture::Get().GetString(m_Text, m_Font);
	//}
}
bool CTextRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// 前準備
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// 描画
	// ﾃｸｽﾁｬの数だけ繰り返す
	Vector2 pos = m_pRectTransform.lock()->GetPos();
	Vector2 scale = m_pRectTransform.lock()->GetSize();

	for (auto tex : m_pTexList)
	{
		CPolygon::SetColor(GetColor(1));
		CPolygon::SetAlpha(GetColor().a);
		CPolygon::SetAngle(m_pRectTransform.lock()->GetAngle());
		CPolygon::SetSize({ tex.Width * scale.x, tex.Height * scale.y });
		CPolygon::SetPos(pos);
		if (tex.pTex)
			CPolygon::SetTexture(tex.pTex);
		else
			CPolygon::SetTexture(NULL);
		CPolygon::Draw(CDXDevice::Get().GetDeviceContext());

		// 縦書きか
		if(!m_bVerticalWrit)
			pos.x += tex.Width * scale.x;
		else
			pos.y -= tex.Height * scale.y;
	}

	// 設定の初期化
	CPolygon::SetColor(0, 0, 0, 1);
	CPolygon::SetAngle(0);
	CPolygon::SetSize(1, 1);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetTexture(NULL);

	CDXDevice::Get().SetZBuffer(true);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	return true;
}

#ifdef BUILD_MODE

void CTextRenderer::ImGuiDebug()
{
	ImGui::Text(u8"textRenderer");
	std::string text;
	text = MySpace::Debug::InputString(text, u8"String");
	m_Text = StringToWString(text);
	
	ImGui::InputFloat(u8"オフセット", &m_fOffset);
	ImGui::Checkbox(u8"縦", &m_bVerticalWrit);
}
std::wstring CTextRenderer::VKeyToWString(int nKey)
{
	static std::map<int, std::wstring> retMap =
	{
		{ 'A', L"A" },
		{ 'B', L"B" },
		{ 'C', L"C" },
		{ 'D', L"D" },
		{ 'E', L"E" },
		{ 'F', L"F" },
		{ 'G', L"G" },
		{ 'H', L"H" },
		{ 'I', L"I" },
		{ 'J', L"J" },
		{ 'K', L"K" },
		{ 'L', L"L" },
		{ 'M', L"M" },
		{ 'N', L"N" },
		{ 'O', L"O" },
		{ 'P', L"P" },
		{ 'Q', L"Q" },
		{ 'R', L"R" },
		{ 'S', L"S" },
		{ 'T', L"T" },
		{ 'U', L"U" },
		{ 'V', L"V" },
		{ 'W', L"W" },
		{ 'X', L"X" },
		{ 'Y', L"Y" },
		{ 'Z', L"Z" },
	};
	// 存在すればキーが返り、なければempty
	return retMap.at(nKey);
}
#endif // BUILD_MODE
