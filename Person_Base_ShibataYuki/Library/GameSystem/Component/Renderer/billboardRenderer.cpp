//=========================================================
// [billboardRenderer.h]
// �쐬: 2022/07/04
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Manager/sceneManager.h>

#include <Application/Application.h>

#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//--- �萔��`
#define TEXTURE_PATH		FORDER_DIR(Data/Texture/)


//==========================================================
// �R���X�g���N�^
//==========================================================
CBillboardRenderer::CBillboardRenderer(std::shared_ptr<CGameObject> owner)
	: CMeshRenderer(owner)
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(1.0f, 1.0f, 1.0f, 0.3f),
		Vector4(1.0f, 1.0f, 1.0f, 0.0f),	// w��ø����L���Ɏg���Ă���
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),	// w��power�Ɏg���Ă���
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBillboardRenderer::~CBillboardRenderer()
{
	m_pBillboard->Fin();
	m_pBillboard.reset();
	m_pSprite.reset();
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CBillboardRenderer::OnLoad()
{
	CRenderer::OnLoad();
	m_pBillboard->Init();
	m_pSprite->SetImage(m_pSprite->GetImageName());
}

//==========================================================
// �������Ăяo��
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
// ������
//==========================================================
void CBillboardRenderer::Init()
{
	// �`��˗�
	CMeshRenderer::Init();
}

//==========================================================
// �X�V
//==========================================================
void CBillboardRenderer::Update()
{
	if (!m_pBillboard)
		return;

	// ��Ұ��ݍX�V
	m_pSprite->Update();
	
	// ���W�X�V
	m_pBillboard->Update(Transform()->GetPos(),Vector2(Transform()->GetScale().x, Transform()->GetScale().y));
}

//==========================================================
// �`��
//==========================================================
bool CBillboardRenderer::Draw()
{
	if (!CMeshRenderer::Draw())
		return false;

	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	//CDXDevice::Get()->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	CLight::GetMain()->SetDisable();
	
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
		m_pBillboard->SetTextureMatrix(mtxTexture);

		m_MeshMaterial.m_Ambient.w = 1.0f;	// ø����t���O�ƂȂ��Ă���
		SetInstancing(m_pBillboard.get(), m_pSprite->GetImageName());
	}
	else
	{
		m_MeshMaterial.m_Ambient.w = 0.0f;	// ø����t���O�ƂȂ��Ă���
		SetInstancing(m_pBillboard.get(), std::string());
	}

	//CDXDevice::Get()->SetZBuffer(true);			
	CLight::GetMain()->SetEnable();		// �����L��
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����
	
	return true;
}

//=========================================================
//
//=========================================================
RENDER_DATA CBillboardRenderer::GetShaderData()
{
	XMUINT4 vFlags(1, 0, 0, 0);

	// w��power�Ɏg���Ă���
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

	//--- �摜�̃����[�h
	Debug::SetTextAndAligned("Image Reload");
	if (s_FileList.empty() || ImGui::Button("##Image Reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	//--- ���j���[����ø����I��
	Debug::SetTextAndAligned("Image");
	if (auto name = DispComboSelect(s_FileList, "##Image", m_pSprite->GetImageName()); !name.empty())
	{
		m_pSprite->SetImage(name);
	}

	m_pSprite->ImGuiDebug();

	CMeshRenderer::ImGuiDebug();
}

#endif // BUILD_MODE