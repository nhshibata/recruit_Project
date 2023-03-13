//==========================================================
// [negative.cpp]
//---------------------------------------------------------
// 作成:2023/03/07
//---------------------------------------------------------
//==========================================================

#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/PostProcess/negative.h>
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
CNegative::CNegative()
	:m_fNega(0)
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);
}

//=========================================================
// デストラクタ
//=========================================================
CNegative::~CNegative()
{

}

//=========================================================
// 静的関数
//=========================================================
HRESULT CNegative::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_Negative.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_Negative", ps);
	}

	return hr;
}

//=========================================================
// 描画実行
//=========================================================
void CNegative::DrawSprite(CGBuffer* pGBuf)
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
	// 定数
	XMFLOAT4 rate(m_fNega, 0, 0, 0);
	pSM->CBWrite(NAME_TO(SHADER_RATE), &rate);
	pSM->BindCB(NAME_TO(SHADER_RATE));
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	CPolygon::Draw(pDX->GetDeviceContext(), "PS_Negative", "VS_2D");

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
ID3D11ShaderResourceView* CNegative::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void CNegative::ImGuiDebug()
{
	Debug::SetTextAndAligned(u8"Nega 反転率");
	ImGui::DragFloat("##Nega反転率", &m_fNega);
}

#endif // BIULD_MODE