//=========================================================
// [renderer.cpp] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルード部
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


// コンストラクタ
CPolygonRenderer::CPolygonRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner)
{
	
}
CPolygonRenderer::~CPolygonRenderer()
{
	m_pSprite.reset();
}
void CPolygonRenderer::Awake()
{
	GetOwner()->SetLayer(CLayer::E_Layer::UI);
	// コンポーネントの取得
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();
	
	// なければ追加
	if (!m_pRectTransform.lock())
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}

	// ﾃｸｽﾁｬの追加
	m_pSprite = std::make_shared<CSpriteAnimation>();
}
void CPolygonRenderer::Init()
{
	// 描画依頼
	CRenderer::Init();
}
void CPolygonRenderer::Update()
{
	// ｱﾆﾒｰｼｮﾝ更新
	m_pSprite->Update();
	//std::shared_ptr<CPolygonRenderer> ptr = std::make_shared<CPolygonRenderer>(GetPtr().lock().get());
}
bool CPolygonRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// 前準備
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// 描画
	CPolygon::SetColor(GetColor(1));
	//CPolygon::SetAlpha(GetColor().w);
	CPolygon::SetAngle(m_pRectTransform.lock()->GetAngle());
	CPolygon::SetSize(m_pRectTransform.lock()->GetSize());
	CPolygon::SetPos(m_pRectTransform.lock()->GetPos());
	CPolygon::SetUV((XMFLOAT2)m_pSprite->GetUV());
	CPolygon::SetFrameSize((XMFLOAT2)m_pSprite->GetFrameSize());
	if(m_pSprite->GetImage().lock())
		CPolygon::SetTexture(m_pSprite->GetImage().lock()->GetSRV());
	else
		CPolygon::SetTexture(NULL);
	CPolygon::Draw(CDXDevice::Get().GetDeviceContext());

	// 設定の初期化
	CPolygon::SetColor(0,0,0,1);
	CPolygon::SetAngle(0);
	CPolygon::SetSize(1,1);
	CPolygon::SetPos(0,0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetTexture(NULL);
	
	CDXDevice::Get().SetZBuffer(true);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));

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
	/*ImGui::InputFloat2(u8"ﾃｸｽﾁｬサイズ", (float*)&m_pSprite->GetFrameSize(), 1);
	ImGui::InputFloat2(u8"ﾃｸｽﾁｬ座標", (float*)&m_pSprite->GetUV(), 1);*/
	ImGui::InputFloat4(u8"色", (float*)&color, 1);
	SetColor(color);
	
	m_pSprite->ImGuiDebug();

	if (s_FileList.empty() || ImGui::Button("画像 reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	// ﾃｸｽﾁｬ
	if (auto name = DispMenuBar(s_FileList, "画像"); !name.empty())
	{
		m_pSprite->SetImage(name);
	}
}
#endif // BUILD_MODE