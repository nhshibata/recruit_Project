//==========================================================
// [GBuffer.cpp]
//---------------------------------------------------------
// 作成:2023/02/15
//---------------------------------------------------------
// オフスクリーンレンダリング用
// 設定を行い、ﾃｸｽﾁｬ取得なども行う
// StackCameraに使う
//==========================================================

//--- インクルード部
#include <Application/Application.h>
#include <Application/screen.h>
#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Render/polygon.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
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
// デストラクタ
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
// 描画前設定
//==========================================================
void CGBuffer::SetUpMultiRenderTarget()
{
	m_aRenderTaget[int(ETexture::COLOR)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::NORMAL)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::WORLD)]->Clear(1, 1, 1, 1);

	m_pDepthStencil->Clear();

	//--- マルチレンダーターゲットの設定
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11RenderTargetView* aView[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetView(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetView(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetView(),
	};
	pDC->OMSetRenderTargets(int(ETexture::MAX), aView, m_pDepthStencil->GetView());

}

//==========================================================
// ﾃｸｽﾁｬ設定
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
// 描画
//==========================================================
void CGBuffer::DrawSpriteRender(const ETexture eTex, float width, float height)
{

	//--- 前準備
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	//--- 描画設定
	CPolygon::SetSize(width, height);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ﾃｸｽﾁｬ設定
	CPolygon::SetTexture(GetRenderTexture(eTex));
	//--- 描画
	CPolygon::Draw(pDX->GetDeviceContext());

	//--- 設定の初期化
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);

	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

}
