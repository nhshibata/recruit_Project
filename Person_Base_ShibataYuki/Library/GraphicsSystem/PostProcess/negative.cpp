//==========================================================
// [negative.cpp]
//---------------------------------------------------------
// μ¬:2023/03/07
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
// RXgN^
//=========================================================
CNegative::CNegative()
	:m_fNega(0)
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);
}

//=========================================================
// fXgN^
//=========================================================
CNegative::~CNegative()
{

}

//=========================================================
// ΓIΦ
//=========================================================
HRESULT CNegative::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szNegative));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szNegative, ps);
	}

	return hr;
}

//=========================================================
// `ζΐs
//=========================================================
void CNegative::DrawSprite(CGBuffer* pGBuf)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// θ
	XMFLOAT4 rate(m_fNega, 0, 0, 0);
	pSM->CBWrite(NAME_TO(SHADER_RATE), &rate);
	pSM->BindCB(NAME_TO(SHADER_RATE));

	CPostProcess::DrawSprite(pGBuf, CPixelName::szNegative, CVertexName::sz2D);

}


#if BUILD_MODE

void CNegative::ImGuiDebug()
{
	Debug::SetTextAndAligned(u8"Nega ½]¦");
	ImGui::DragFloat("##Nega½]¦", &m_fNega);
}

#endif // BIULD_MODE