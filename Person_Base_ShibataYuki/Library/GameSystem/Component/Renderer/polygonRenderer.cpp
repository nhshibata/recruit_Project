//=========================================================
// [renderer.cpp] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Transform/transform.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Render/polygon.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;


//==========================================================
// コンストラクタ
//==========================================================
CPolygonRenderer::CPolygonRenderer()
{
	m_pSprite = std::make_shared<CSpriteAnimation>();
}

//==========================================================
// コンストラクタ
//==========================================================
CPolygonRenderer::CPolygonRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner)
{
	m_pSprite = std::make_shared<CSpriteAnimation>();
}

//==========================================================
// デストラクタ
//==========================================================
CPolygonRenderer::~CPolygonRenderer()
{
	m_pSprite.reset();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CPolygonRenderer::Awake()
{
	GetOwner()->SetLayer(CLayer::E_Layer::UI);

	// ﾃｸｽﾁｬの追加
	if (!m_pSprite)
		m_pSprite = std::make_shared<CSpriteAnimation>();

	// コンポーネントの取得
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();

	// なければ追加
	if (!m_pRectTransform.lock())
	{
		auto rect = GetOwner()->AddComponent<CRectTransform>();
		m_pRectTransform = rect;
		rect->SetSize(100, 100);
	}
}

//==========================================================
// 初期化
//==========================================================
void CPolygonRenderer::Init()
{
	// 描画依頼
	CRenderer::Init();
}

//==========================================================
// 更新
//==========================================================
void CPolygonRenderer::Update()
{
	// ｱﾆﾒｰｼｮﾝ更新
	m_pSprite->Update();
	//std::shared_ptr<CPolygonRenderer> ptr = std::make_shared<CPolygonRenderer>(GetPtr().lock().get());
}

//==========================================================
// 描画
//==========================================================
bool CPolygonRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// 前準備
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// 描画
	CPolygon::SetColor(GetColor(1));
	//CPolygon::SetAlpha(GetColor().a);
	CPolygon::SetAngle(m_pRectTransform.lock()->GetAngle());
	CPolygon::SetSize(m_pRectTransform.lock()->GetSize());
	CPolygon::SetPos(m_pRectTransform.lock()->GetPos());
	CPolygon::SetUV((XMFLOAT2)m_pSprite->GetUV());
	CPolygon::SetFrameSize((XMFLOAT2)m_pSprite->GetFrameSize());
	if(m_pSprite->GetImage().lock())
		CPolygon::SetTexture(m_pSprite->GetImage().lock()->GetSRV());
	else
		CPolygon::SetTexture(NULL);
	CPolygon::Draw(pDX->GetDeviceContext());

	// 設定の初期化
	CPolygon::SetColor(0,0,0,1);
	CPolygon::SetAngle(0);
	CPolygon::SetSize(1,1);
	CPolygon::SetPos(0,0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetTexture(NULL);
	
	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	return true;
}


#ifdef BUILD_MODE

void CPolygonRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	Color color = GetColor();
	ImGui::Text(u8"PolygonRenderer");
	ImGui::Text(u8"filaName : %s", m_pSprite->GetImageName().c_str());
	ImGui::InputFloat4(u8"色", (float*)&color);
	SetColor(color);
	
	m_pSprite->ImGuiDebug();

	//--- ファイル内検索
	if (s_FileList.empty() || ImGui::Button(u8"画像 reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	// ﾃｸｽﾁｬ
	if (auto name = DispMenuBar(s_FileList, u8"画像"); !name.empty())
	{
		m_pSprite->SetImage(name);
	}

}

#endif // BUILD_MODE