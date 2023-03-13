//==========================================================
// [renderTarget.cpp]
//----------------------------------------------------------
// �쐬:2022/12/12
//----------------------------------------------------------
//==========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CRenderTarget::CRenderTarget()
	:m_pRTV(nullptr)
{
	
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CRenderTarget::CRenderTarget(DXGI_FORMAT format, UINT width, UINT height)
	:m_pRTV(nullptr)
{
	HRESULT hr = Create(format, width, height);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CRenderTarget::CRenderTarget(IDXGISwapChain* swap, ID3D11Device* device)
	:m_pRTV(nullptr)
{
	HRESULT hr = S_OK;

	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	if (SUCCEEDED(hr))
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.Texture2D.MipSlice = 0;
		hr = device->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
		if (SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			m_pTex->GetDesc(&desc);
		}
	}

	// �쐬�Ɏ��s���Ă�����폜
	if (FAILED(hr))
	{
		return;
	}
}

//==========================================================
// �R�s�[�R���X�g���N�^
//==========================================================
CRenderTarget::CRenderTarget(const CRenderTarget& copy)
{
	this->m_pRTV = copy.m_pRTV;
	this->m_pSRV = copy.m_pSRV;
	this->m_pTex = copy.m_pTex;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CRenderTarget::~CRenderTarget()
{
	Release();
}

//==========================================================
// ����
//==========================================================
HRESULT CRenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	HRESULT hr = S_OK;
	Application* pApp = Application::Get();

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	//--- �e�N�X�`���쐬
	/*D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = nullptr;
	data.SysMemPitch = desc.Width * 4;*/
	hr = pApp->GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTex);
	if (FAILED(hr))
	{
		return hr;
	}

	//--- ø����ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// ����
	hr = pApp->GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);
	if (FAILED(hr))
	{
		return hr;
	}

	//--- �����_�[�^�[�Q�b�g�ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	//--- ����
	hr = pApp->GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

//==========================================================
// �������
//==========================================================
void CRenderTarget::Release()
{
	CImageResource::Unload();
	if (m_pRTV)
	{
		m_pRTV->Release();
		m_pRTV = nullptr;
	}
}

//==========================================================
// �����_�[ �ް����Z�b�g
//==========================================================
void CRenderTarget::Clear(float* color)
{
	Application::Get()->GetDeviceContext()->ClearRenderTargetView(GetView(), color);
}

//==========================================================
// �����_�[ �ް����Z�b�g
//==========================================================
void CRenderTarget::Clear(float r, float g, float b, float a)
{
	float color[] = { r,g,b,a };
	Application::Get()->GetDeviceContext()->ClearRenderTargetView(GetView(), color);
}