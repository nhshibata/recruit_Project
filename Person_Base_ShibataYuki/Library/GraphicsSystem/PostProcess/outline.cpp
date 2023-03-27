//==========================================================
// [outline.cpp]
//---------------------------------------------------------
// 作成:2023/03/07
//---------------------------------------------------------
//==========================================================

//--- インクルード部
#include <Application/Application.h>

#include <GraphicsSystem/PostProcess/outline.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <GraphicsSystem/Render/polygon.h>
#include <Application/screen.h>

using namespace MySpace::Graphics;

//=========================================================
// コンストラクタ
//=========================================================
COutline::COutline()
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);
}

//=========================================================
// デストラクタ
//=========================================================
COutline::~COutline()
{

}

//=========================================================
// 静的関数
//=========================================================
HRESULT COutline::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szOutline));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szOutline, ps);
	}

	return hr;
}

//=========================================================
// 描画
//=========================================================
void COutline::DrawSprite(CGBuffer* pGBuf)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	const auto screenSize = CScreen::GetSize();
	ID3D11RenderTargetView* pView[] = { m_pMainRT.GetView() };

	//レンダリングターゲットをクリア。
	m_pMainRT.Clear(0, 0, 0, 0);

	//--- 描画設定
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(screenSize);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetColor(1, 1, 1, 1);

	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	CPolygon::Draw(pDX->GetDeviceContext(), CPixelName::szOutline, CVertexName::sz2D);

	//--- 設定の初期化
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);
	pDX->SetZBuffer(true);
	pDX->SetZWrite(true);
	pDX->SetBlendState(EBlendState::BS_NONE);

}

//=========================================================
// SRV
//=========================================================
ID3D11ShaderResourceView* COutline::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void COutline::ImGuiDebug()
{

}

#endif // BIULD_MODE