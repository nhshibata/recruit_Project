//=========================================================
// [polygonRenderer.cpp] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
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
	:m_nZValue(0)
{
	m_pSprite = std::make_unique<CSpriteAnimation>();
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CPolygonRenderer::CPolygonRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner)
	, m_nZValue(0)
{
	m_pSprite = std::make_unique<CSpriteAnimation>();
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CPolygonRenderer::~CPolygonRenderer()
{
	m_pSprite.reset();
}

//==========================================================
// �������Ăяo��
//==========================================================
#pragma optimize("", off)
void CPolygonRenderer::Awake()
{
	// ø����̒ǉ�
	if (!m_pSprite)
		m_pSprite = std::make_unique<CSpriteAnimation>();

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
#pragma optimize("", on)

//==========================================================
// ������
//==========================================================
void CPolygonRenderer::Init()
{
	// �`��˗�
	//CRenderer::Init();
	Transform()->Update();

	m_nDrawIdx = SceneManager::CSceneManager::Get().GetDrawSystem()->PolygonRegist(BaseToDerived<CPolygonRenderer>());
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

//==========================================================
// Z
//==========================================================
void CPolygonRenderer::SetZ(const int z)
{
	m_nZValue = z;
	SceneManager::CSceneManager::Get().GetDrawSystem()->SortOn();
}

//==========================================================
// Z
//==========================================================
void CPolygonRenderer::SetZ(const EZValue z)
{
	m_nZValue = static_cast<int>(z);
	SceneManager::CSceneManager::Get().GetDrawSystem()->SortOn();
}

#ifdef BUILD_MODE

void CPolygonRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileList;

	CRenderer::ImGuiDebug();
	
	ImGui::Text(u8"PolygonRenderer");
	ImGui::Text(u8"filaName : %s", m_pSprite->GetImageName().c_str());
	
	m_pSprite->ImGuiDebug();

	int z = m_nZValue;
	if (ImGui::InputInt("Z", &z))
	{
		SetZ(z);
	}

	//--- �t�@�C��������
	if (s_FileList.empty() || ImGui::Button(u8"�摜 reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	// ø���
	if (auto name = DispMenuBar(s_FileList, u8"�摜"); !name.empty())
	{
		m_pSprite->SetImage(name);
	}

}

#endif // BUILD_MODE