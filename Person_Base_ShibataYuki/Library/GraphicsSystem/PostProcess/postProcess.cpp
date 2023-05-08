//==========================================================
// [negative.cpp]
//---------------------------------------------------------
// 作成:2023/03/07
//---------------------------------------------------------
//==========================================================

#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/PostProcess/postProcess.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Render/polygon.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Graphics;

//=========================================================
// コンストラクタ
//=========================================================
CPostProcess::CPostProcess()
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);
}

//=========================================================
// デストラクタ
//=========================================================
CPostProcess::~CPostProcess()
{

}

//=========================================================
// 描画実行
//=========================================================
void CPostProcess::DrawSprite(CGBuffer* pGBuf)
{
}

//=========================================================
// 描画実行
//=========================================================
void CPostProcess::DrawSprite(CGBuffer* pGBuf, std::string ps, std::string vs)
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
	//pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	pDX->SetBlendState(EBlendState::BS_ADDITIVE);
	CPolygon::SetSize(screenSize);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::SetColor(1, 1, 1, 1);

	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	CPolygon::Draw(pDX->GetDeviceContext(), ps, vs);

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
ID3D11ShaderResourceView* CPostProcess::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void CPostProcess::ImGuiDebug()
{
	
}

#endif // BIULD_MODE