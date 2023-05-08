//==========================================================
// [outline.cpp]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

//--- �C���N���[�h��
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
// �R���X�g���N�^
//=========================================================
COutline::COutline()
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);
}

//=========================================================
// �f�X�g���N�^
//=========================================================
COutline::~COutline()
{

}

//=========================================================
// �ÓI�֐�
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
// �`��
//=========================================================
void COutline::DrawSprite(CGBuffer* pGBuf)
{

	CPostProcess::DrawSprite(pGBuf, CPixelName::szOutline, CVertexName::sz2D);

}


#if BUILD_MODE

void COutline::ImGuiDebug()
{

}

#endif // BIULD_MODE