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
#include <GraphicsSystem/Shader/shaderStruct.h>

using namespace MySpace::Graphics;

#ifndef NAME_TO
#define NAME_TO(name)	#name
#endif // !NAME_TO

//=========================================================
// �V�F�[�_�[�ݒ�
//=========================================================
HRESULT CGaussianBlur::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	VertexShaderSharedPtr vsY = std::make_shared<CVertexShader>();
	VertexShaderSharedPtr vsX = std::make_shared<CVertexShader>();
	ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();

	hr = ps->Make(CPixelName::GetCSO(CPixelName::szBlur));
	if (FAILED(hr))
		return hr;
	pSM->SetPS(CPixelName::szBlur, ps);

	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = vsX->Make(CVertexName::GetCSO(CVertexName::szXBlur), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(CVertexName::szXBlur, vsX);

	hr = vsY->Make(CVertexName::GetCSO(CVertexName::szYBlur), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(CVertexName::szYBlur, vsY);

	hr = cb->Make(sizeof(WEIGHT_TABLE), Slot::CB_BLUR, CConstantBuffer::EType::Pixel);
	if (FAILED(hr))
		return hr;
	pSM->SetCB(NAME_TO(WEIGHT_TABLE), cb);

	return hr;
}

//=========================================================
// �R���X�g���N�^
//=========================================================
CGaussianBlur::CGaussianBlur()
{
	
}

//=========================================================
// �����t���R���X�g���N�^
//=========================================================
CGaussianBlur::CGaussianBlur(
	ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT eFormat,
	float fWidth, float fHeight)
{
	Init(originalTexture, eFormat, fWidth, fHeight);
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CGaussianBlur::~CGaussianBlur()
{
}

//=========================================================
// ������
//=========================================================
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

	// �d�݃e�[�u���̌v�Z
	CalcWeightsTableFromGaussian(m_weight.fWeights, NUM_WEIGHTS, 8.0f);
}

//=========================================================
// �����_�[�^�[�Q�b�g�̏�����
//=========================================================
void CGaussianBlur::InitRenderTargets()
{
#if 0
	//X�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_xBlurRenderTarget.Create(
		m_eFormat,
		(UINT)fWidth / 2,
		(UINT)fHeight
	);

	//Y�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_yBlurRenderTarget.Create(
		m_eFormat,
		(UINT)fWidth / 2,
		(UINT)fHeight / 2
	);
#else
	//X�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_xBlurRenderTarget.Create(
		m_eFormat,
		(UINT)m_fWidth,
		(UINT)m_fHeight
	);

	//Y�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_yBlurRenderTarget.Create(
		m_eFormat,
		(UINT)m_fWidth,
		(UINT)m_fHeight
	);
#endif // 0

}

//=========================================================
// �`��
//=========================================================
void CGaussianBlur::ExecuteOnGPU(float blurPower, ID3D11ShaderResourceView* pTex)
{
	m_originalTexture = pTex;

	//--- �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ADDITIVE));

	//�d�݃e�[�u�����X�V����
	UpdateWeightsTable(blurPower);

	//--- ���u���[�����s
	//�����_�����O�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* pView[] = { m_xBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//�����_�����O�^�[�Q�b�g���N���A
	m_xBlurRenderTarget.Clear(0, 0, 0, 0);
	//�h���[
	DrawSpriteXBlur();

	//�c�u���[�����s
	//�����_�����O�^�[�Q�b�g��ݒ�
	pView[0] = { m_yBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//�����_�����O�^�[�Q�b�g���N���A
	m_yBlurRenderTarget.Clear(0, 0, 0, 0);
	//�h���[
	DrawSpriteYBlur();

	//--- �ݒ��߂�
	pDX->SetZBuffer(true);
	pDX->SetZWrite(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	pDX->GetDeviceContext()->RSSetViewports(1, pDX->GetViewPort());
}

//=========================================================
// X�`��
//=========================================================
void CGaussianBlur::DrawSpriteXBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();	

	//���u���[�p�̃X�v���C�g������������
	{
		// �X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ���
		/*D3D11_VIEWPORT vp;
		vp.MaxDepth = 1;
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight;
		pDC->RSSetViewports(1, &vp);*/

		// ���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);
		pSM->BindCB(NAME_TO(WEIGHT_TABLE));
	}

	//--- �`��ݒ�
	CPolygon::SetSize(m_fWidth, m_fHeight);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ø����ݒ�
	// 0�Ԗ�
	CPolygon::SetTexture(m_originalTexture);
	//--- �`��
	CPolygon::Draw(pDC, CPixelName::szBlur, CVertexName::szXBlur);

	//--- �ݒ�̏�����
	CPolygon::SetSize(1, 1);

}

//=========================================================
// Y�`��
//=========================================================
void CGaussianBlur::DrawSpriteYBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();
	
	//�c�u���[�p�̃X�v���C�g������������B
	{
		// �X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ����B
		/*D3D11_VIEWPORT vp;
		vp.MaxDepth = 1;
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight / 2;
		pDC->RSSetViewports(1, &vp);*/

		// ���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);
	}

	//--- �`��ݒ�
	CPolygon::SetSize(m_fWidth, m_fHeight);
	// ø����ݒ�
	// 0�Ԗ�
	CPolygon::SetTexture(m_xBlurRenderTarget.GetSRV());
	//--- �`��
	CPolygon::Draw(pDC, CPixelName::szBlur, CVertexName::szYBlur);

	//--- �ݒ�̏�����
	CPolygon::SetSize(1, 1);
}

//=========================================================
// �d�݌v�Z
//=========================================================
void CGaussianBlur::UpdateWeightsTable(float blurPower)
{
	if (blurPower == 0.0f)
		return;

#if 1
	float total = 0.0f;
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		float x = static_cast<float>(i - NUM_WEIGHTS / 2);
		m_weight.fWeights[i] = expf(-(x * x) / (2.0f * blurPower));
		if (i == 0)
		{
			total += m_weight.fWeights[i];
		}
		else
		{
			total += 2.0f * m_weight.fWeights[i];
		}
	}
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] /= total;
	}

#else
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
#endif // 1

}


/// <summary>
/// �K�E�V�A���֐��𗘗p���ďd�݃e�[�u�����v�Z����
/// </summary>
/// <param name="weightsTbl">�d�݃e�[�u���̋L�^��</param>
/// <param name="sizeOfWeightsTbl">�d�݃e�[�u���̃T�C�Y</param>
/// <param name="sigma">���U��B���̐��l���傫���Ȃ�ƕ��U��������Ȃ�</param>
void CGaussianBlur::CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{
	// �d�݂̍��v���L�^����ϐ����`����
	float total = 0;

	// ��������K�E�X�֐���p���ďd�݂��v�Z���Ă���
	// ���[�v�ϐ���x����e�N�Z������̋���
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// �d�݂̍��v�ŏ��Z���邱�ƂŁA�d�݂̍��v��1�ɂ��Ă���
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}