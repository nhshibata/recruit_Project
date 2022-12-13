//==========================================================
// [renderTarget.h]
//-------------------------
// 作成:2022/12/12
//-------------------------
//==========================================================

//--- インクルード部
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::Graphics;


CRenderTarget::CRenderTarget(DXGI_FORMAT format, UINT width, UINT height)
{
	HRESULT hr = Create(format, width, height);
}
CRenderTarget::~CRenderTarget()
{
	Release();
}

HRESULT CRenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	HRESULT hr = S_OK;

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

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = nullptr;
	data.SysMemPitch = desc.Width * 4;
	hr = CDXDevice::Get().GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTex);
	if (FAILED(hr))
	{
		return hr;
	}

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// 生成
	hr = CDXDevice::Get().GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);
	if (FAILED(hr))
	{
		return hr;
	}

	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// 生成
	hr = CDXDevice::Get().GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

void CRenderTarget::Release()
{
	CImageResource::Unload();
	m_pRTV->Release();
	m_pRTV = nullptr;
}

void CRenderTarget::Clear(float* color)
{
	CDXDevice::Get().GetDeviceContext()->ClearRenderTargetView(GetView(), color);
}