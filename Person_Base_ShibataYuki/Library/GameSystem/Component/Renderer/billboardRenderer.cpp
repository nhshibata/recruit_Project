//=========================================================
// [billboardRenderer.h]
// �쐬: 2022/07/04
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Light/directionalLight.h>

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
	m_pBillboard->Fin();
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
	// �`��˗�
	CMeshRenderer::Init();
}
void CBillboardRenderer::Update()
{
	if (!m_pBillboard)
		return;

	// ��Ұ��ݍX�V
	m_pSprite->Update();
	
	// ���W�X�V
	m_pBillboard->Update(Transform()->GetPos(),Vector2(Transform()->GetScale().x, Transform()->GetScale().y));
}
bool CBillboardRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;

	//CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	CLight::Get()->SetDisable();
	
	// ø����}�b�s���O�X�V
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
		//XMStoreFloat4x4(&mtxTexture, XMMatrixIdentity());
		m_pBillboard->SetTextureMatrix(mtxTexture);
		SetInstancing(m_pBillboard.get(), m_pSprite->GetImageName());
	}
	else
	{
		SetInstancing(m_pBillboard.get());
	}
	//CDXDevice::Get().SetZBuffer(true);			
	CLight::Get()->SetEnable();// �����L��
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����
	
	return true;
}

#ifdef BUILD_MODE

void CBillboardRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	// polygon�Ƃقړ����c
	Color color = GetColor();
	ImGui::Text(u8"PolygonRenderer");
	ImGui::Text(u8"filaName : %s", m_pSprite->GetImageName().c_str());
	ImGui::InputFloat4(u8"�F", (float*)&color);
	SetColor(color);

	m_pSprite->ImGuiDebug();

	// �摜�̃����[�h
	if (s_FileList.empty() || ImGui::Button(u8"�摜 reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	//--- ���j���[����ø����I��
	if (auto name = DispMenuBar(s_FileList, u8"�摜"); !name.empty())
	{
		m_pSprite->SetImage(name);
	}
}
#endif // BUILD_MODE