//==========================================================
// [GBuffer.cpp]
//---------------------------------------------------------
// �쐬:2023/02/15
//---------------------------------------------------------
// �I�t�X�N���[�������_�����O�p
// �ݒ���s���Aø����擾�Ȃǂ��s��
// StackCamera�Ɏg��
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <Application/screen.h>
#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Render/polygon.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGBuffer::CGBuffer()
{
	UINT x, y;
	x = (UINT)CScreen::GetWidth();
	y = (UINT)CScreen::GetHeight();
	
	m_aRenderTaget.resize(int(ETexture::MAX));
	m_aRenderTaget[int(ETexture::COLOR)] = std::make_unique<CRenderTarget>(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, x, y);
	m_aRenderTaget[int(ETexture::NORMAL)] = std::make_unique<CRenderTarget>(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, x, y);
	m_aRenderTaget[int(ETexture::WORLD)] = std::make_unique<CRenderTarget>(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, x, y);

	m_pDepthStencil = std::make_unique<CDepthStencil>(x, y, true);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGBuffer::~CGBuffer()
{
	for (auto & rt : m_aRenderTaget)
	{
		rt.reset();
	}
	m_pDepthStencil.reset();
}

//==========================================================
// �`��O�ݒ�
//==========================================================
void CGBuffer::SetUpMultiRenderTarget()
{
	m_aRenderTaget[int(ETexture::COLOR)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::NORMAL)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::WORLD)]->Clear(1, 1, 1, 1);

	m_pDepthStencil->Clear();

	//--- �}���`�����_�[�^�[�Q�b�g�̐ݒ�
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11RenderTargetView* aView[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetView(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetView(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetView(),
	};
	pDC->OMSetRenderTargets(int(ETexture::MAX), aView, m_pDepthStencil->GetView());

}

//==========================================================
// ø����ݒ�
//==========================================================
void CGBuffer::SetUpTextures()
{
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11ShaderResourceView* const aTex[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetSRV(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetSRV(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetSRV(),
	};
	pDC->PSSetShaderResources(6, 1, &aTex[0]);
	pDC->PSSetShaderResources(7, 1, &aTex[1]);
	pDC->PSSetShaderResources(8, 1, &aTex[2]);

}

//==========================================================
// �`��
//==========================================================
void CGBuffer::DrawSpriteRender(const ETexture eTex, float width, float height)
{

	//--- �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//--- �`��ݒ�
	CPolygon::SetSize(width, height);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ø����ݒ�
	CPolygon::SetTexture(GetRenderTexture(eTex));
	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext());

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);

	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

}
