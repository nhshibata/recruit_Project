//==========================================================
// [bloom.cpp]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// シェーダーの魔導書から対応
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

//--- インクルード部
#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

using namespace MySpace::Graphics;

CBloom::CBloom()
	:m_fBlurPower(20)
{
	m_pLuminnceRT.Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);

}

CBloom::~CBloom()
{

}

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

void CBloom::DrawSprite(ID3D11ShaderResourceView* originalTexture)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto screenSize = CScreen::GetSize();
	ID3D11RenderTargetView* pView[] = { m_pLuminnceRT.GetView()};

	//レンダリングターゲットをクリア。
	m_pLuminnceRT.Clear(0, 0, 0, 0);
	m_pMainRT.Clear(0, 0, 0, 0);

	//--- 描画設定
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	CPolygon::SetSize(screenSize);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	
	//--- 輝度抽出画像生成
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_Luminance", "VS_2D");

	//--- 輝度ﾃｸｽﾁｬからぼけ画像生成
	m_Gauss.Init(m_pLuminnceRT.GetSRV(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, screenSize.x, screenSize.y);
	m_Gauss.ExecuteOnGPU(m_fBlurPower);

	// ﾒｲﾝの画像と輝度抽出後画像を加算合成
	pView[0] = m_pMainRT.GetView();
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);

	//--- 描画
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_ColorTexture", "VS_2D");
	
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ADDITIVE));

	// 輝度抽出後のﾃｸｽﾁｬを設定
	CPolygon::SetTexture(m_Gauss.GetBokeTexture());
	//--- 描画
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_2D", "VS_2D");

	//--- 設定の初期化
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);
	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

}

ID3D11ShaderResourceView* CBloom::GetResource()
{
	return m_pMainRT.GetSRV();
}