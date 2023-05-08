//=========================================================
// [textRenerer.cpp] 
// �쐬: 2022/08/04
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
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
// �R���X�g���N�^
//==========================================================
CTextRenderer::CTextRenderer(std::shared_ptr<CGameObject> owner)
	:CPolygonRenderer(owner), m_fOffset(0), m_Font(L"�l�r �o����"), m_bVerticalWrit(false),
	m_Text(std::wstring()), m_uOldTextSize(0)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CTextRenderer::~CTextRenderer()
{
	CRenderer::~CRenderer();
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CTextRenderer::OnLoad()
{
	m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);
	CPolygonRenderer::OnLoad();
}

//==========================================================
// �������Ăяo��
//==========================================================
void CTextRenderer::Awake()
{
#if BUILD_MODE
	// ImGui�̓s����A���O��v�͔����������߁AID��t������
	// ��������
	GetOwner()->SetName("Text_" + std::to_string(++m_nBulidCreateNum));
#else
	// ��������
	GetOwner()->SetName("Text");
#endif // BUILD_MODE

	m_Text = L"Text";

	SetZ(CPolygonRenderer::EZValue::DEFAULT);

	// �R���|�[�l���g�̎擾
	// �Ȃ���Βǉ�
	if(m_pRectTransform = GetOwner()->GetComponent<CRectTransform>(); !m_pRectTransform.lock()) 
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}
	
}

//==========================================================
// ������
//==========================================================
void CTextRenderer::Init()
{
	if (m_pRectTransform = GetOwner()->GetComponent<CRectTransform>(); !m_pRectTransform.lock())
	{
		m_pRectTransform = GetOwner()->AddComponent<CRectTransform>();
	}

	m_pRectTransform.lock()->SetSize(1, 1);

	// �T�C�Y�擾
	m_uOldTextSize = m_Text.size();
	
	// �����񂩂�ø����擾
	m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);

	// �p��������̕`��˗�
	CPolygonRenderer::Init();
}

//==========================================================
// �X�V
//==========================================================
void CTextRenderer::Update()
{
	// �T�C�Y��r
	if (m_uOldTextSize != m_Text.size())
	{
		m_aTexList = Application::Get()->GetSystem<CAssetsManager>()->GetFont()->GetString(m_Text, m_Font);
		m_uOldTextSize = m_Text.size();
	}
}

//==========================================================
// �`��
//==========================================================
bool CTextRenderer::Draw()
{
	if (!CRenderer::Draw())return false;

	// �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//--- �V�F�[�_�[
	auto pSM = Application::Get()->GetSystem<MySpace::Graphics::CAssetsManager>()->GetShaderManager();
	pSM->CallBackFuncAndBind(m_strPixelShader, m_strVertexShader);

	//--- �`��
	// ø����̐������J��Ԃ�
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

		// �c������
		if(!m_bVerticalWrit)
			pos.x += tex.Width * scale.x;
		else
			pos.y -= tex.Height * scale.y;
	}

	// �ݒ�̏�����
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
// �e�L�X�g�ݒ�
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
// �ȗ��p
// ���z�L�[�𕶎���Ƃ��Ď擾
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
	// ���݂���΃L�[���Ԃ�A�Ȃ����empty
	return retMap.at(nKey);
}

#endif // BUILD_MODE
