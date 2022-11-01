//=========================================================
// [renderer.cpp] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
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


// �R���X�g���N�^
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
	// �R���|�[�l���g�̎擾
	m_pRectTransform = GetOwner()->GetComponent<CRectTransform>();
	
	// �Ȃ���Βǉ�
	if (!m_pRectTransform.lock())
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}

	// ø����̒ǉ�
	m_pSprite = std::make_shared<CSpriteAnimation>();
}
void CPolygonRenderer::Init()
{
	// �`��˗�
	CRenderer::Init();
}
void CPolygonRenderer::Update()
{
	// ��Ұ��ݍX�V
	m_pSprite->Update();
	//std::shared_ptr<CPolygonRenderer> ptr = std::make_shared<CPolygonRenderer>(GetPtr().lock().get());
}
bool CPolygonRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// �O����
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// �`��
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

	// �ݒ�̏�����
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
	/*ImGui::InputFloat2(u8"ø����T�C�Y", (float*)&m_pSprite->GetFrameSize(), 1);
	ImGui::InputFloat2(u8"ø������W", (float*)&m_pSprite->GetUV(), 1);*/
	ImGui::InputFloat4(u8"�F", (float*)&color, 1);
	SetColor(color);
	
	m_pSprite->ImGuiDebug();

	if (s_FileList.empty() || ImGui::Button("�摜 reload"))
	{
		MySpace::System::CFilePath file;
		s_FileList = file.GetAllFileName(TEXTURE_PATH);
	}

	// ø���
	if (auto name = DispMenuBar(s_FileList, "�摜"); !name.empty())
	{
		m_pSprite->SetImage(name);
	}
}
#endif // BUILD_MODE