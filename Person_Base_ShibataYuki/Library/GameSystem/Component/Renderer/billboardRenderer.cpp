//=========================================================
// [billboardRenderer.h]
// 作成: 2022/07/04
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

#define TEXTURE_PATH		FORDER_DIR(Data/Texture/)

CBillboardRenderer::CBillboardRenderer(std::shared_ptr<CGameObject> owner)
	: CMeshRenderer(owner)
{

}
CBillboardRenderer::~CBillboardRenderer()
{
	m_pBillboard.reset();
	m_pSprite.reset();
}
void CBillboardRenderer::Awake()
{
	if (!m_pBillboard) 
	{
		m_pBillboard = std::make_shared<CBillboard>();
		m_pBillboard->Init();
	}
	if(!m_pSprite)
		m_pSprite = std::make_shared<CSpriteAnimation>();
}
void CBillboardRenderer::Init()
{
	// 描画依頼
	CMeshRenderer::Init();
}
void CBillboardRenderer::Update()
{
	if (!m_pBillboard)
		return;

	// ｱﾆﾒｰｼｮﾝ更新
	m_pSprite->Update();
	
	// 座標更新
	m_pBillboard->Update(Transform()->GetPos(),Vector2(Transform()->GetScale().x, Transform()->GetScale().y));
}
bool CBillboardRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;
	
	m_pBillboard->Draw(m_pSprite->GetTexture(), m_pSprite.get());
	
	return true;
}

#ifdef BUILD_MODE

void CBillboardRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	// polygonとほぼ同じ…
	Color color = GetColor();
	ImGui::Text(u8"PolygonRenderer");
	ImGui::Text(u8"filaName : %s", m_pSprite->GetImageName().c_str());
	ImGui::InputFloat4(u8"色", (float*)&color);
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