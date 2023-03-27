//=========================================================
// [polygonRenderer.cpp] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <GraphicsSystem/Render/polygon.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CPolygonRenderer::CPolygonRenderer()
{
	//m_pSprite = std::make_shared<CSpriteAnimation>();
	
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CPolygonRenderer::CPolygonRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner)
	,m_nZValue((int)EZValue::DEFAULT)
{
	auto rect = GetOwner()->AddComponent<CRectTransform>();
	m_pRectTransform = rect;
	rect->SetSize(100, 100);
	m_strPixelShader = CPixelName::sz2D;
	m_strVertexShader = CVertexName::sz2D;
}

//==========================================================
// �R�s�[�R���X�g���N�^
//==========================================================
//CPolygonRenderer::CPolygonRenderer(const CPolygonRenderer& copy)
//{
//	this->m_pRectTransform = copy.m_pRectTransform;
//	this->m_pSprite = copy.m_pSprite;
//	this->m_nDrawIdx = copy.m_nDrawIdx;
//	this->m_nZValue = copy.m_nZValue;
//}

//==========================================================
// �f�X�g���N�^
//==========================================================
CPolygonRenderer::~CPolygonRenderer()
{

}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CPolygonRenderer::OnLoad()
{
	SetImageName(m_pSprite->GetImageName());
	m_pRectTransform.reset();
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();
	CRenderer::OnLoad();
}

//==========================================================
// �������Ăяo��
//==========================================================
//#pragma optimize("", off)
void CPolygonRenderer::Awake()
{
	m_pSprite = std::make_shared<CSpriteAnimation>();

	// �R���|�[�l���g�̎擾
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();

	// �Ȃ���Βǉ�
	if (!m_pRectTransform.lock())
	{
		auto rect = GetOwner()->AddComponent<CRectTransform>();
		m_pRectTransform = rect;
		rect->SetSize(100, 100);
	}
}
//#pragma optimize("", on)

//==========================================================
// ������
//==========================================================
void CPolygonRenderer::Init()
{
	if(!m_pSprite)
		m_pSprite = std::make_shared<CSpriteAnimation>();

	// �`��˗�
	//CRenderer::Init();
	Transform()->Update();

	if(auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem(); sys)
		m_nDrawIdx = sys->PolygonRegist(BaseToDerived<CPolygonRenderer>());
}

//==========================================================
// �X�V
//==========================================================
void CPolygonRenderer::Update()
{
	// ��Ұ��ݍX�V
	m_pSprite->Update();
	//std::shared_ptr<CPolygonRenderer> ptr = std::make_shared<CPolygonRenderer>(GetPtr().lock().get());
}

//==========================================================
// �`��
//==========================================================
bool CPolygonRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	//--- �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//--- �V�F�[�_�[
	auto pSM = Application::Get()->GetSystem<MySpace::Graphics::CAssetsManager>()->GetShaderManager();
	pSM->CallBackFuncAndBind(m_strPixelShader, m_strVertexShader);

	//--- �`��ݒ�
	CPolygon::SetColor(GetColor(1));	// rgba
	CPolygon::SetAlpha(GetColor().a);	// rgba
	CPolygon::SetAngle(m_pRectTransform.lock()->GetAngle());
	CPolygon::SetSize(m_pRectTransform.lock()->GetSize());
	CPolygon::SetPos(m_pRectTransform.lock()->GetPos());
	CPolygon::SetUV((XMFLOAT2)m_pSprite->GetUV());
	CPolygon::SetFrameSize((XMFLOAT2)m_pSprite->GetFrameSize());
	//--- ø����ݒ�
	if(m_pSprite->GetImage().lock())
		CPolygon::SetTexture(m_pSprite->GetImage().lock()->GetSRV());
	else
		CPolygon::SetTexture(NULL);
	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext());

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
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

CSpriteAnimation* CPolygonRenderer::GetSprite() 
{
	return m_pSprite.get(); 
}

std::shared_ptr<CRectTransform> CPolygonRenderer::GetRectTransform() 
{
	return m_pRectTransform.lock(); 
}

std::string CPolygonRenderer::GetImageName()
{
	return m_pSprite->GetImageName(); 
}

void CPolygonRenderer::SetImageName(std::string name)
{
	m_pSprite->SetImage(name);
}

//==========================================================
// Z
//==========================================================
void CPolygonRenderer::SetZ(const int z)
{
	m_nZValue = z;
	SceneManager::CSceneManager::Get()->GetDrawSystem()->SortOn();
}

//==========================================================
// Z
//==========================================================
void CPolygonRenderer::SetZ(const EZValue z)
{
	m_nZValue = static_cast<int>(z);
	SceneManager::CSceneManager::Get()->GetDrawSystem()->SortOn();
}

#ifdef BUILD_MODE

void CPolygonRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	CRenderer::ImGuiDebug();
	
	ImGui::Separator();

	int z = m_nZValue;
	Debug::SetTextAndAligned("ZValue");
	if (ImGui::InputInt("##value", &z))
	{
		SetZ(z);
	}

	Debug::SetTextAndAligned("BG");
	if(ImGui::Button("BG"))
		SetZ((int)EZValue::BG);
	ImGui::SameLine();

	Debug::SetTextAndAligned("Default");
	if(ImGui::Button("Default"))
		SetZ((int)EZValue::DEFAULT);

	Debug::SetTextAndAligned("FOG");
	if(ImGui::Button("Fog"))
		SetZ((int)EZValue::FOG);
	
	//--- �t�@�C��������
	Debug::SetTextAndAligned("Image Reload");
	if (s_FileList.empty() || ImGui::Button("##Image Reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	// ø���
	Debug::SetTextAndAligned("Image");
	if (auto name = DispComboSelect(s_FileList, "##Image", m_pSprite->GetImageName()); !name.empty())
	{
		m_pSprite->SetImage(name);
	}

	ImGui::Separator();

	m_pSprite->ImGuiDebug();

}

#endif // BUILD_MODE