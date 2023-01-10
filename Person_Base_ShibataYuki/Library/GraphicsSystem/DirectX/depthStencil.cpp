//==========================================================
// [depthStencil.cpp]
//-------------------------
// 作成:2022/12/12
//-------------------------
//==========================================================

//--- インクルード部
#include <GraphicsSystem/DirectX/depthStencil.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CDepthStencil::CDepthStencil(UINT width, UINT height, bool useStencil)
	:m_pDSV(nullptr)
{
	HRESULT hr = Create(width, height, useStencil);
}

//==========================================================
// デストラクタ
//==========================================================
CDepthStencil::~CDepthStencil()
{

}

//==========================================================
// 生成
//==========================================================
HRESULT CDepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	HRESULT hr = S_OK;
	Application* pApp = Application::Get();

	//--- 情報設定
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	// ステンシル使用判定
	bool bUseStencil = (desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT);

	// リソース生成
	desc.Format = bUseStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
	//--- テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = nullptr;
	data.SysMemPitch = desc.Width * 4;
	hr = pApp->GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTex);
	//--- 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// 生成
	hr = pApp->GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);
	
	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = bUseStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	//--- 生成
	hr = pApp->GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);

	return hr;
}

//==========================================================
// 解放処理
//==========================================================
void CDepthStencil::Release()
{
	if (!m_pDSV)
		return;
	m_pDSV->Release();
	m_pDSV = nullptr;
}

//==========================================================
// 深度ﾃﾞｰﾀクリア
//==========================================================
void CDepthStencil::Clear()
{
	Application::Get()->GetDeviceContext()->ClearDepthStencilView(GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}