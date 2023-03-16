//==========================================================
// [GBuffer.cpp]
//---------------------------------------------------------
// �쐬:2023/02/15
//---------------------------------------------------------
// �I�t�X�N���[�������_�����O�p
// �ݒ���s���Aø����擾�Ȃǂ��s��
// StackCamera�Ɏg��
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <Application/screen.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>

#include <GraphicsSystem/DirectX/GBuffer.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
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
	m_aRenderTaget[int(ETexture::DEPTH)] = std::make_unique<CRenderTarget>(DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, x, y);
	
	m_pDepthStencil = std::make_unique<CDepthStencil>(x, y, true);
}

//==========================================================
// �f�X�g���N�^
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
// �`��O�ݒ�
//==========================================================
void CGBuffer::SetUpMultiRenderTarget()
{
	//--- �`����N���A
	m_aRenderTaget[int(ETexture::COLOR)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::NORMAL)]->Clear(0, 0, 0, 0);
	m_aRenderTaget[int(ETexture::WORLD)]->Clear(1, 1, 1, 1);
	m_aRenderTaget[int(ETexture::DEPTH)]->Clear(1, 1, 1, 1);

	m_pDepthStencil->Clear();

	//--- �}���`�����_�[�^�[�Q�b�g�ݒ�
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11RenderTargetView* aView[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetView(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetView(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetView(),
		m_aRenderTaget[int(ETexture::DEPTH)]->GetView(),
	};
	pDC->OMSetRenderTargets(int(ETexture::MAX), aView, m_pDepthStencil->GetView());

	//--- �萔�o�b�t�@��������
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	//--- ���
	//XMFLOAT4X4 mat[4];
	//auto pCam = CCamera::GetMain();
	//mat[0] = pCam->Transform()->GetLocalMatrix().Transpose();
	//mat[1] = pCam->GetViewMatrix().Transpose();
	////mat[2] = pCam->GetProjMatrix().Transpose();
	//mat[2] = CCamera::CalcProjMatrix(45.0f, CScreen::GetWidth() / CScreen::GetHeight(), 10.2f, 100.0f).Transpose();
	//XMStoreFloat4x4(&mat[3], XMMatrixIdentity()); // �g��Ȃ�dummy

	//--- ���C�g
	CDirectionalLight* light = dynamic_cast<CDirectionalLight*>(CLight::GetMain());
	if (!light)
		return;

	auto sunDir = light->GetDir();
	// �r���[
	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(50, 300, -3, 0),
		DirectX::XMVectorSet(sunDir.x, sunDir.y, sunDir.z, 0),
		DirectX::XMVectorSet(0, 1, 0, 0)
	);
	// ���s���e
	DirectX::XMMATRIX sunProj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45), CScreen::GetWidth() / CScreen::GetHeight(), 0.2f, 1000.0f);
	// Orthographic Projection�s����쐬����
	//float nearPlane = 0.1f;
	//float farPlane = 1000.0f;
	//sunProj = XMMatrixOrthographicOffCenterLH(0, CScreen::GetWidth(), 0, CScreen::GetHeight(), nearPlane, farPlane);

	SHADER_SUN sunMat;
	DirectX::XMStoreFloat4x4(&sunMat.sunView, DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&sunMat.sunProj, DirectX::XMMatrixTranspose(sunProj));
	// 1,2�Ԗڂ�n��
	sm->CBWrite(NAME_TO(SHADER_SUN), &sunMat);
	sm->BindCB(NAME_TO(SHADER_SUN));

	sm->GetPS("PS_GBuffer")->Bind();

}

//==========================================================
// �`��O�ݒ�
//==========================================================
void CGBuffer::SetUpColorRenderTarget()
{
	//--- �`����N���A
	//m_aRenderTaget[int(ETexture::COLOR)]->Clear(0, 0, 0, 0);

	//--- �}���`�����_�[�^�[�Q�b�g�ݒ�
	auto pDC = Application::Get()->GetDeviceContext();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	ID3D11RenderTargetView* aView[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetView(),
	};
	//pDC->OMSetRenderTargets(1, aView, pDX->GetDepthStencilView());
	pDC->OMSetRenderTargets(1, aView, m_pDepthStencil->GetView());
}

//==========================================================
// ø����ݒ�
//==========================================================
void CGBuffer::SetUpTextures()
{
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11ShaderResourceView* const aTex[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetSRV(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetSRV(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetSRV(),
		m_aRenderTaget[int(ETexture::DEPTH)]->GetSRV(),
	};

	pDC->PSSetShaderResources(6, 1, &aTex[int(ETexture::COLOR)]);
	pDC->PSSetShaderResources(7, 1, &aTex[int(ETexture::NORMAL)]);
	pDC->PSSetShaderResources(8, 1, &aTex[int(ETexture::WORLD)]);
	pDC->PSSetShaderResources(9, 1, &aTex[int(ETexture::DEPTH)]);
	//pDC->PSSetShaderResources(4, 1, &aTex[int(ETexture::DEPTH)]);

}

//==========================================================
// 
//==========================================================
void CGBuffer::CopyTexture()
{
	//--- Colorø����̂݁A�X�N���[���`�悳�ꂽ�摜�̎擾
	auto pDC = Application::Get()->GetDeviceContext();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	ID3D11Texture2D* resource = nullptr;
	ID3D11Texture2D* copy = nullptr;

	resource = pDX->GetRenderTexture();
	copy = m_aRenderTaget[int(ETexture::COLOR)]->GetTexter();
	pDC->CopyResource(copy, resource);
}

//==========================================================
// �X�v���C�g�̕`��
//==========================================================
void CGBuffer::DrawSpriteRender(const ETexture eTex, float width, float height)
{
	//--- �O����
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZBuffer(false);

	//--- �`��ݒ�
	CPolygon::SetSize(width, height);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ø����ݒ�
	CPolygon::SetTexture(GetRenderTexture(eTex));

	//--- �`��
	CPolygon::Draw(pDX->GetDeviceContext());

	//--- �ݒ�̏�����
	CPolygon::SetColor(1, 1, 1, 1);
	CPolygon::SetSize(1, 1);
	CPolygon::SetTexture(NULL);

	pDX->SetZBuffer(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

}

//=========================================================
// ø����ݒ�
//=========================================================
void CGBuffer::SetSRV(const ETexture e)
{
	auto pDC = Application::Get()->GetDeviceContext();
	ID3D11ShaderResourceView* const aTex[] = {
		m_aRenderTaget[int(ETexture::COLOR)]->GetSRV(),
		m_aRenderTaget[int(ETexture::NORMAL)]->GetSRV(),
		m_aRenderTaget[int(ETexture::WORLD)]->GetSRV(),
		m_aRenderTaget[int(ETexture::DEPTH)]->GetSRV(),
	};

	switch (e)
	{
		case ETexture::COLOR:
			pDC->PSSetShaderResources(6, 1, &aTex[int(ETexture::COLOR)]);
			break;
		case ETexture::NORMAL:
			pDC->PSSetShaderResources(7, 1, &aTex[int(ETexture::NORMAL)]);
			break;
		case ETexture::WORLD:
			pDC->PSSetShaderResources(8, 1, &aTex[int(ETexture::WORLD)]);
			break;
		case ETexture::DEPTH:
			pDC->PSSetShaderResources(9, 1, &aTex[int(ETexture::DEPTH)]);
			break;
		default:
			break;
	}
}


#if BUILD_MODE

void CGBuffer::ImGuiDebug()
{
	static bool disp = true;
	static ImVec2 size = ImVec2(CScreen::GetWidth() / 10, CScreen::GetHeight() / 10);

	ImGui::Checkbox("GBuffer View", &disp);
	
	if (!disp)
		return;

	if (ImGui::Begin("view 3", &disp))
	{
		ID3D11ShaderResourceView* const aTex[] = {
			m_aRenderTaget[int(ETexture::COLOR)]->GetSRV(),
			m_aRenderTaget[int(ETexture::NORMAL)]->GetSRV(),
			m_aRenderTaget[int(ETexture::WORLD)]->GetSRV(),
			m_aRenderTaget[int(ETexture::DEPTH)]->GetSRV(),
			Application::Get()->GetSystem<CDXDevice>()->GetSRV(),
		};

		Debug::SetTextAndAligned("ViewSize", -50);
		ImGui::DragFloat2("##View", (float*)&size);

		ImGui::Text("RenderTarget Color");
		ImGui::Image(aTex[int(ETexture::COLOR)], size);
		ImGui::Text("RenderTarget Normal");
		ImGui::Image(aTex[int(ETexture::NORMAL)], size);
		ImGui::Text("RenderTarget World");
		ImGui::Image(aTex[int(ETexture::WORLD)], size);
		ImGui::Text("RenderTarget Depth");
		ImGui::Image(aTex[int(ETexture::DEPTH)], size);
		ImGui::Text("RenderTarget Screen");
		ImGui::Image(aTex[int(ETexture::DEPTH)+1], size);
		
		ImGui::End();
	}

}

#endif // BUILD_MODE
