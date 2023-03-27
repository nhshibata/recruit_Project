//=========================================================
// [billboardRenderer.h]
// 作成: 2022/07/04
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>

#include <Application/Application.h>

#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//--- 定数定義
#define TEXTURE_PATH		FORDER_DIR(Data/Texture/)


//==========================================================
// コンストラクタ
//==========================================================
CBillboardRenderer::CBillboardRenderer(std::shared_ptr<CGameObject> owner)
	: CMeshRenderer(owner)
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(1.0f, 1.0f, 1.0f, 0.3f),
		Vector4(1.0f, 1.0f, 1.0f, 0.0f),	// wはﾃｸｽﾁｬ有無に使われている
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),	// wはpowerに使われている
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CBillboardRenderer::~CBillboardRenderer()
{
	m_pBillboard->Fin();
	m_pBillboard.reset();
	m_pSprite.reset();
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CBillboardRenderer::OnLoad()
{
	CRenderer::OnLoad();
	m_pBillboard->Init();
	m_pSprite->SetImage(m_pSprite->GetImageName());
}

//==========================================================
// 生成時呼び出し
//==========================================================
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

//==========================================================
// 初期化
//==========================================================
void CBillboardRenderer::Init()
{
	// 描画依頼
	CMeshRenderer::Init();
}

//==========================================================
// 更新
//==========================================================
void CBillboardRenderer::Update()
{
	if (!m_pBillboard)
		return;

	// ｱﾆﾒｰｼｮﾝ更新
	m_pSprite->Update();
	
	// 座標更新
	m_pBillboard->Update(Transform()->GetPos(),Vector2(Transform()->GetScale().x, Transform()->GetScale().y));
}

//==========================================================
// 描画
//==========================================================
bool CBillboardRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;

	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	//CDXDevice::Get()->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	CLight::GetMain()->SetDisable();
	
	// ﾃｸｽﾁｬマッピング更新
	if (m_pSprite->GetTexture() && m_pSprite)
	{
		auto uvSize = m_pSprite->GetFrameSize();
		auto uv = m_pSprite->GetUV();
		XMFLOAT4X4 mtxTexture;
		XMMATRIX mtxTex = XMMatrixScaling(
			uvSize.x,
			uvSize.y, 1.0f);
		mtxTex = XMMatrixMultiply(mtxTex, XMMatrixTranslation(
			uv.x,
			uv.y, 0.0f));
		XMStoreFloat4x4(&mtxTexture, mtxTex);
		m_pBillboard->SetTextureMatrix(mtxTexture);

		m_MeshMaterial.m_Ambient.w = 1.0f;	// ﾃｸｽﾁｬフラグとなっている
		SetInstancing(m_pBillboard.get(), m_pSprite->GetImageName());
	}
	else
	{
		m_MeshMaterial.m_Ambient.w = 0.0f;	// ﾃｸｽﾁｬフラグとなっている
		SetInstancing(m_pBillboard.get(), std::string());
	}

	//CDXDevice::Get()->SetZBuffer(true);			
	CLight::GetMain()->SetEnable();		// 光源有効
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// αブレンディング無効
	
	return true;
}

//=========================================================
//
//=========================================================
RENDER_DATA CBillboardRenderer::GetShaderData()
{
	XMUINT4 vFlags(1, 0, 0, 0);

	// wはpowerに使われている
	Vector4 spec(m_MeshMaterial.m_Specular.x, m_MeshMaterial.m_Specular.y, m_MeshMaterial.m_Specular.z, m_MeshMaterial.m_Power);
	return RENDER_DATA(
		Transform()->GetWorldMatrix(),
		m_MeshMaterial.m_Ambient, m_MeshMaterial.m_Diffuse,
		spec, m_MeshMaterial.m_Emissive,
		vFlags);
}

#ifdef BUILD_MODE

void CBillboardRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	//--- 画像のリロード
	Debug::SetTextAndAligned("Image Reload");
	if (s_FileList.empty() || ImGui::Button("##Image Reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	//--- メニューからﾃｸｽﾁｬ選択
	Debug::SetTextAndAligned("Image");
	if (auto name = DispComboSelect(s_FileList, "##Image", m_pSprite->GetImageName()); !name.empty())
	{
		m_pSprite->SetImage(name);
	}

	m_pSprite->ImGuiDebug();

	CMeshRenderer::ImGuiDebug();
}

#endif // BUILD_MODE