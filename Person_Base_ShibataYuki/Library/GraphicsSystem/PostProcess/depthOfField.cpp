//==========================================================
// [depthOfField.cpp]
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
#include <GraphicsSystem/PostProcess/depthOfField.h>
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
CDepthOfField::CDepthOfField()
	:m_fPower(5)
{
	auto screenSize = CScreen::GetSize();

	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);

	m_Gauss.Init(nullptr, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, screenSize.x, screenSize.y);
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CDepthOfField::~CDepthOfField()
{

}

//=========================================================
// �ÓI�����o�֐�
//=========================================================
HRESULT CDepthOfField::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	PixelShaderSharedPtr psLumi = std::make_shared<CPixelShader>();
	PixelShaderSharedPtr psColor = std::make_shared<CPixelShader>();

	hr = psLumi->Make(CSO_PATH(PS_DepthOfField.cso));
	if (FAILED(hr))
		return hr;
	hr = psColor->Make(CSO_PATH(PS_ColorTexture.cso));
	if (FAILED(hr))
		return hr;

	pSM->SetPS("PS_DepthOfField", psLumi);
	pSM->SetPS("PS_ColorTexture", psColor);

	return hr;
}

//=========================================================
// �X�v���C�g�`��
//=========================================================
void CDepthOfField::DrawSprite(CGBuffer* pGBuf)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	const auto screenSize = CScreen::GetSize();
	

	//�����_�����O�^�[�Q�b�g���N���A�B
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


	//--- ø�������ڂ��摜����
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	m_Gauss.ExecuteOnGPU(m_fPower, pGBuf->GetRenderTexture(CGBuffer::ETexture::COLOR));

	//--- �`��ݒ�
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(screenSize);

	// ���Z����
	pDX->SetBlendState(EBlendState::BS_ADDITIVE);

	// Ҳ݂̉摜�ƋP�x���o��摜�����Z����	
	{
		ID3D11RenderTargetView* pView[] = { m_pMainRT.GetView() };
		pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	}
	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext(), CPixelName::szColorTexture, CVertexName::sz2D);

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
ID3D11ShaderResourceView* CDepthOfField::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void CDepthOfField::ImGuiDebug()
{
	static bool viewDisp = true;

	Debug::SetTextAndAligned("DepthOfField Power");
	ImGui::DragFloat("##DepthOfField Power", &m_fPower, 0.1f);

	ImGui::Checkbox("bloom view", &viewDisp);

	if (!viewDisp)
		return;

	ImVec2 size = ImVec2(CScreen::GetWidth() / 5, CScreen::GetHeight() / 5);
	//ImGui::SetNextWindowSize(size, ImGuiCond_::ImGuiCond_Once);

	if (ImGui::Begin("bloom window", &viewDisp))
	{
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