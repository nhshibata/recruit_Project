//==========================================================
// [bloom.cpp]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// �V�F�[�_�[�̖���������Ή�
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Graphics;

//=========================================================
// �R���X�g���N�^
//=========================================================
CBloom::CBloom()
	:m_fBlurPower(20), m_fThreshold(0.0f)
{
	auto screenSize = CScreen::GetSize();

	m_pLuminnceRT.Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, (UINT)screenSize.x, (UINT)screenSize.y);
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);

	m_Gauss.Init(m_pLuminnceRT.GetSRV(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, screenSize.x, screenSize.y);
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CBloom::~CBloom()
{

}

//=========================================================
// �ÓI�����o�֐�
//=========================================================
HRESULT CBloom::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	PixelShaderSharedPtr psLumi = std::make_shared<CPixelShader>();
	PixelShaderSharedPtr psColor = std::make_shared<CPixelShader>();

	hr = psLumi->Make(CSO_PATH(PS_Luminance.cso));
	if (FAILED(hr))
		return hr;
	hr = psColor->Make(CSO_PATH(PS_ColorTexture.cso));
	if (FAILED(hr))
		return hr;

	pSM->SetPS("PS_Luminance", psLumi);
	pSM->SetPS("PS_ColorTexture", psColor);
	
	return hr;
}

//=========================================================
// �X�v���C�g�`��
//=========================================================
void CBloom::DrawSprite(CGBuffer* pGBuf)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	const auto screenSize = CScreen::GetSize();
	ID3D11RenderTargetView* pView[] = { m_pLuminnceRT.GetView()};

	//�����_�����O�^�[�Q�b�g���N���A�B
	m_pLuminnceRT.Clear(0, 0, 0, 0);
	m_pMainRT.Clear(0, 0, 0, 0);

	//--- �`��ݒ�
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(screenSize);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetColor(1, 1, 1, 1);
	
	//--- �P�x���o�摜����
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	// �萔
	XMFLOAT4 rate(m_fThreshold, 0, 0, 0);
	pSM->CBWrite(NAME_TO(SHADER_RATE), &rate);
	pSM->BindCB(NAME_TO(SHADER_RATE));
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_Luminance", "VS_2D");

	//--- �P�xø�������ڂ��摜����
	m_Gauss.ExecuteOnGPU(m_fBlurPower, m_pLuminnceRT.GetSRV());

	//--- �`��ݒ�
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(screenSize);

	// ���Z����
	pDX->SetBlendState(EBlendState::BS_ADDITIVE);
	
	// Ҳ݂̉摜�ƋP�x���o��摜�����Z����	
	{
		pView[0] = m_pMainRT.GetView();
		pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	}
	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_ColorTexture", "VS_2D");

	// �P�x���o���ø�����ݒ�
	CPolygon::SetTexture(m_Gauss.GetBokeTexture());
	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext());

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);
	pDX->SetZBuffer(true);
	pDX->SetZWrite(true);
	pDX->SetBlendState(EBlendState::BS_NONE);

}

//=========================================================
// �����_�[ø����̎擾
//=========================================================
ID3D11ShaderResourceView* CBloom::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void CBloom::ImGuiDebug() 
{
	static bool viewDisp = true;

	Debug::SetTextAndAligned("Bloom Power");
	ImGui::DragFloat("##Bloom Power", &m_fBlurPower, 0.1f);

	Debug::SetTextAndAligned("Bloom Threshold");
	ImGui::DragFloat("##Bloom Threshold", &m_fThreshold, 0.01f, -1.0f, 1.0f);
	
	ImGui::Checkbox("bloom view", &viewDisp);

	if (!viewDisp)
		return;
	
	ImVec2 size = ImVec2(CScreen::GetWidth() / 5, CScreen::GetHeight() / 5);
	//ImGui::SetNextWindowSize(size, ImGuiCond_::ImGuiCond_Once);

	if (ImGui::Begin("bloom window", &viewDisp))
	{
		ImGui::Text(u8"�P�x");
		ImGui::Image(m_pLuminnceRT.GetSRV(), size);
		ImGui::Text(u8"���u���[");
		ImGui::Image(m_Gauss.GetXBlurTexture(), size);
		ImGui::Text(u8"�c�u���[");
		ImGui::Image(m_Gauss.GetBokeTexture(), size);
		ImGui::Text(u8"bloom��������");
		ImGui::Image(m_pMainRT.GetSRV(), size);

		ImGui::End();
	}

};

#endif // BIULD_MODE