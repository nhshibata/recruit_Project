//==========================================================
// [gaussianBlur.cpp]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// �V�F�[�_�[�̖���������Ή�
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

//--- �C���N���[�h��
#include <math.h>
#include <d3d11.h>
#include <Application/Application.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

using namespace MySpace::Graphics;

#ifndef NAME_TO
#define NAME_TO(name)	#name
#endif // !NAME_TO

void CGaussianBlur::Init(
	ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT eFormat,
	float Width,
	float Height)
{
	m_originalTexture = originalTexture;
	m_fWidth = Width;
	m_fHeight = Height;
	m_eFormat = eFormat;

	//�����_�����O�^�[�Q�b�g���������B
	InitRenderTargets();
	//�X�v���C�g���������B
	DrawSpriteXBlur();
}


void CGaussianBlur::ExecuteOnGPU(float blurPower)
{
	//--- �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ADDITIVE));

	//�d�݃e�[�u�����X�V����B
	UpdateWeightsTable(blurPower);

	//--- ���u���[�����s�B
	//�����_�����O�^�[�Q�b�g��ݒ�B
	ID3D11RenderTargetView* pView[] = { m_xBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//�����_�����O�^�[�Q�b�g���N���A�B
	m_xBlurRenderTarget.Clear(0, 0, 0, 0);
	//�h���[�B
	DrawSpriteXBlur();

	//�c�u���[�����s�B
	//�����_�����O�^�[�Q�b�g��ݒ�B
	pView[0] = { m_yBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//�����_�����O�^�[�Q�b�g���N���A�B
	m_yBlurRenderTarget.Clear(0, 0, 0, 0);
	//�h���[�B
	DrawSpriteYBlur();

	//--- �ݒ��߂�
	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	D3D11_VIEWPORT vp;
	vp.MaxDepth = pDX->GetViewPort()->MaxDepth;
	vp.Width = pDX->GetViewPort()->Width;
	vp.Height = pDX->GetViewPort()->Height;
	pDX->GetDeviceContext()->RSSetViewports(1, &vp);
}

HRESULT CGaussianBlur::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	VertexShaderSharedPtr vsY = std::make_shared<CVertexShader>();
	VertexShaderSharedPtr vsX = std::make_shared<CVertexShader>();
	ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
	
	hr = ps->Make(CSO_PATH(PS_Blur.cso));
	if (FAILED(hr))
		return hr;
	pSM->SetPS(NAME_TO(PS_Blur), ps);

	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	hr = vsX->Make(CSO_PATH(VS_XBlur.cso), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(NAME_TO(VS_XBlur), vsX);

	hr = vsY->Make(CSO_PATH(VS_YBlur.cso), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(NAME_TO(VS_YBlur), vsY);

	hr = cb->Make(sizeof(WEIGHT_TABLE), 9, CConstantBuffer::EType::Pixel);
	if (FAILED(hr))
		return hr;
	pSM->SetCB(NAME_TO(WEIGHT_TABLE), cb);

	return hr;
}

void CGaussianBlur::InitRenderTargets()
{
	//X�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_xBlurRenderTarget.Create(
		m_eFormat,
		m_fWidth / 2,
		m_fHeight
	);
	//DXGI_FORMAT_D32_FLOAT

	//Y�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_yBlurRenderTarget.Create(
		m_eFormat,
		m_fWidth / 2,
		m_fHeight / 2
	);
}

void CGaussianBlur::DrawSpriteXBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();

	D3D11_VIEWPORT vp;
	vp.MaxDepth = 1;
	
	pSM->BindVS(NAME_TO(VS_XBlur));
	pSM->BindCB(NAME_TO(WEIGHT_TABLE));

	//���u���[�p�̃X�v���C�g������������B
	{
		pSM->BindPS(NAME_TO(PS_Blur));

		// �X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ����B
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight;
		pDC->RSSetViewports(1, &vp);

		// ���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);

	}

	//--- �`��ݒ�
	CPolygon::SetSize(m_fWidth / 2, m_fHeight);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ø����ݒ�
	CPolygon::SetTexture(m_originalTexture);
	//--- �`��
	CPolygon::Draw(pDC);

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);

}

void CGaussianBlur::DrawSpriteYBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();

	D3D11_VIEWPORT vp;
	vp.MaxDepth = 1;
	
	pSM->BindVS(NAME_TO(VS_YBlur));
	pSM->BindCB(NAME_TO(WEIGHT_TABLE));

	//�c�u���[�p�̃X�v���C�g������������B
	{
		pSM->BindPS(NAME_TO(PS_Blur));

		// �X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ����B
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight / 2;
		pDC->RSSetViewports(1, &vp);

		// ���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);
	}

	//--- �`��ݒ�
	CPolygon::SetSize(m_fWidth / 2, m_fHeight / 2);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ø����ݒ�
	CPolygon::SetTexture(m_xBlurRenderTarget.GetSRV());
	//--- �`��
	CPolygon::Draw(pDC);

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);
}

void CGaussianBlur::UpdateWeightsTable(float blurPower)
{
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] = expf(-0.5f * (float)(i * i) / blurPower);
		total += 2.0f * m_weight.fWeights[i];

	}
	// �K�i��
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] /= total;
	}
}