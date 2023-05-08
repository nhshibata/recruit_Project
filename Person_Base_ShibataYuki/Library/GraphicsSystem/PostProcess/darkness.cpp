//==========================================================
// [darkness.cpp]
//---------------------------------------------------------
// �쐬:2023/04/09
//---------------------------------------------------------
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/PostProcess/darkness.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

using namespace MySpace::Graphics;

//=========================================================
// �R���X�g���N�^
//=========================================================
CDarkness::CDarkness()
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);

}

//=========================================================
// �f�X�g���N�^
//=========================================================
CDarkness::~CDarkness()
{

}

//=========================================================
// �ÓI�֐�
//=========================================================
HRESULT CDarkness::InitShader()
{
	HRESULT hr = S_OK;
	{
		auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szDarkness));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szDarkness, ps);
	}

	return hr;
}

//=========================================================
// �`��
//=========================================================
void CDarkness::DrawSprite(CGBuffer* pGBuf)
{
	// �e�N�X�`���ݒ�
	pGBuf->SetSRV(CGBuffer::ETexture::COLOR);
	pGBuf->SetSRV(CGBuffer::ETexture::NORMAL);
	
	//CPolygon::Draw(pDX->GetDeviceContext(), CPixelName::szMonochrome, CVertexName::sz2D);

	CPostProcess::DrawSprite(pGBuf, CPixelName::szDarkness, CVertexName::sz2D);

}


#if BUILD_MODE

void CDarkness::ImGuiDebug()
{

}

#endif // BIULD_MODE