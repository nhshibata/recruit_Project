//=========================================================
// [textRenerer.cpp] 
// 作成: 2022/08/04
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/GameObject/gameObject.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Render/polygon.h>

#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/Util/stringConvert.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CTextRenderer::CTextRenderer(std::shared_ptr<CGameObject> owner)
	:CPolygonRenderer(owner), m_fOffset(0), m_Font(L"ＭＳ Ｐ明朝"), m_bVerticalWrit(false),
	m_Text(std::wstring()), m_uOldTextSize(0)
{

}

//==========================================================
// デストラクタ
//==========================================================
CTextRenderer::~CTextRenderer()
{
	CRenderer::~CRenderer();
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CTextRenderer::OnLoad()
{
	m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);
	CPolygonRenderer::OnLoad();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CTextRenderer::Awake()
{
#if BUILD_MODE
	// ImGuiの都合上、名前一致は避けたいため、IDを付け足す
	// 初期化文
	GetOwner()->SetName("Text_" + std::to_string(++m_nBulidCreateNum));
#else
	// 初期化文
	GetOwner()->SetName("Text");
#endif // BUILD_MODE

	m_Text = L"Text";

	SetZ(CPolygonRenderer::EZValue::DEFAULT);

	// コンポーネントの取得
	// なければ追加
	if(m_pRectTransform = GetOwner()->GetComponent<CRectTransform>(); !m_pRectTransform.lock()) 
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}
	
}

//==========================================================
// 初期化
//==========================================================
void CTextRenderer::Init()
{
	if (m_pRectTransform = GetOwner()->GetComponent<CRectTransform>(); !m_pRectTransform.lock())
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}

	m_pRectTransform.lock()->SetSize(1, 1);

	// サイズ取得
	m_uOldTextSize = m_Text.size();
	
	// 文字列からﾃｸｽﾁｬ取得
	m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);

	// 継承元からの描画依頼
	CPolygonRenderer::Init();
}

//==========================================================
// 更新
//==========================================================
void CTextRenderer::Update()
{
	// サイズ比較
	if (m_uOldTextSize != m_Text.size())
	{
		m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);
		m_uOldTextSize = m_Text.size();
	}
}

//==========================================================
// 描画
//==========================================================
bool CTextRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// 前準備
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//--- シェーダー
	auto pSM = Application::Get()->GetSystem<MySpace::Graphics::CAssetsManager>()->GetShaderManager();
	pSM->CallBackFuncAndBind(m_strPixelShader, m_strVertexShader);

	//--- 描画
	// ﾃｸｽﾁｬの数だけ繰り返す
	Vector2 pos = m_pRectTransform.lock()->GetPos();
	Vector2 scale = m_pRectTransform.lock()->GetSize();

	for (auto tex : m_aTexList)
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
		CPolygon::Draw(pDX->GetDeviceContext());

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

	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	return true;
}

//==========================================================
// テキスト設定
//==========================================================
void CTextRenderer::SetTextWString(std::wstring text) 
{
	m_Text = text;
	m_aTexList = Application::Get()->GetSystem<CFontTexture>()->GetString(m_Text, m_Font);
}


#ifdef BUILD_MODE

void CTextRenderer::ImGuiDebug()
{
	std::string text = WStringToString(m_Text);

	Debug::SetTextAndAligned("String");
	auto inputStr = MySpace::Debug::InputString(text, "##String");
	if(inputStr != text)
	{
		m_Text = StringToWString(inputStr);
	}

	
	Debug::SetTextAndAligned("Text offset");
	ImGui::InputFloat("##Text offset", &m_fOffset);
	
	Debug::SetTextAndAligned("Vertical");
	ImGui::Checkbox("##Vertical", &m_bVerticalWrit);

	CRenderer::ImGuiDebug();
}

//==========================================================
// 簡略用
// 仮想キーを文字列として取得
//==========================================================
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
