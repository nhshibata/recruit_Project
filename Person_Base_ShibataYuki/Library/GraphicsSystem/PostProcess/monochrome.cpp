//==========================================================
// [outline.cpp]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <Application/screen.h>

#include <GraphicsSystem/PostProcess/monochrome.h>
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
CMonochrome::CMonochrome()
{
	auto screenSize = CScreen::GetSize();
	m_pMainRT.Create(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, (UINT)screenSize.x, (UINT)screenSize.y);

}

//=========================================================
// �f�X�g���N�^
//=========================================================
CMonochrome::~CMonochrome()
{

}

//=========================================================
// �ÓI�֐�
//=========================================================
HRESULT CMonochrome::InitShader()
{
	HRESULT hr = S_OK;
	{
		auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szMonochrome));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szMonochrome, ps);
	}

	// ø����ǂݍ���
	{
		auto imgMgr = Application::Get()->GetSystem<CAssetsManager>()->GetImageManager();
		m_pToon = imgMgr->GetResource(FORDER_DIR(Data/Texture/ramp.png));
	}

	return hr;
}

//=========================================================
// �`��
//=========================================================
void CMonochrome::DrawSprite(CGBuffer* pGBuf)
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	const auto screenSize = CScreen::GetSize();
	ID3D11RenderTargetView* pView[] = { m_pMainRT.GetView() };

	//�����_�����O�^�[�Q�b�g���N���A�B
	m_pMainRT.Clear(0, 0, 0, 0);

	//--- �`��ݒ�
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
	pGBuf->SetSRV(CGBuffer::ETexture::NORMAL);
	auto pTex = m_pToon->GetSRV();
	pDX->GetDeviceContext()->PSSetShaderResources(5, 1, &pTex);
	CPolygon::Draw(pDX->GetDeviceContext(), CPixelName::szMonochrome, CVertexName::sz2D);

	//--- �ݒ�̏�����
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
ID3D11ShaderResourceView* CMonochrome::GetResource()
{
	return m_pMainRT.GetSRV();
}


#if BUILD_MODE

void CMonochrome::ImGuiDebug()
{

}

#endif // BIULD_MODE