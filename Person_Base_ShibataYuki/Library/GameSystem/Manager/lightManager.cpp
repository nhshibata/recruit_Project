//=========================================================
// [lightManager.h]
// 
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Manager/lightManager.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/DirectX/GBuffer.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//========================================================
// �R���X�g���N�^
//========================================================
CLightManager::CLightManager()
{
	m_pRenderTarget = std::make_unique<CRenderTarget>();
	m_aIntMap.clear();
}

//========================================================
// �f�X�g���N�^
//========================================================
CLightManager::~CLightManager()
{
	m_pRenderTarget.reset();
}

//========================================================
// �ÓI
//========================================================
CLightManager* CLightManager::Get()
{
	static CLightManager instance;
	return &instance;
}

//========================================================
// �V�F�[�_�[�ǂݍ���
//========================================================
HRESULT CLightManager::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
	hr = cb->Make(sizeof(SHADER_POINT_LIGHT), Slot::CB_SPOT_LIGHTS, CConstantBuffer::EType::Pixel);
	if (FAILED(hr))
	{
		return hr;
	}
	pSM->SetCB(NAME_TO(SHADER_POINT_LIGHT), cb);

	return hr;
}

//========================================================
// �o�C���h
//========================================================
void CLightManager::BeginRender(CGBuffer* pGBuff)const
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	{
		//--- �`��N���A
		m_pRenderTarget->Clear(0, 0, 0, 0);

		//--- �`���ؑ�
		ID3D11RenderTargetView* view[] = { m_pRenderTarget->GetView() };
		auto pDX = Application::Get()->GetSystem<CDXDevice>();
		pDX->SwitchRender(*view, pDX->GetDepthStencilView());
	}

	//--- �萔�o�b�t�@����
	{
		// DirectionalLight
		SHADER_GLOBAL_CAMERA_LIGHT sg;
		sg.vEye = XMLoadFloat3(&CCamera::GetMain()->Transform()->GetPos());
		CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::GetMain());
		sg.vLightDir = XMLoadFloat3(&pLight->GetDir());
		sg.vLd = XMLoadFloat4(&pLight->GetDiffuse());
		sg.vLa = XMLoadFloat4(&pLight->GetAmbient());
		sg.vLs = XMLoadFloat4(&pLight->GetSpecular());

		// �萔�o�b�t�@�ݒ�
		pSM->CBWrite(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), &sg, sizeof(SHADER_GLOBAL_CAMERA_LIGHT));
		pSM->BindCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT));
	}

	{
		// �|�C���g���C�g�ݒ�
		SHADER_POINT_LIGHT cbLight;
		int lightIdx = 0;
		for (auto & lightComponent : m_aIntMap)
		{			
			auto light = dynamic_cast<CPointLight*>(lightComponent.second.lock().get());
			// �^�m�F
			if (!light)
				continue;

			// ���i�[
			cbLight.aLights[lightIdx] = STPointLight(light->GetRange(), light->GetColor()());

			lightIdx = (lightIdx + 1);
			if (lightIdx >= SHADER_POINT_LIGHT::MAX_LIGHT)
				break;
		}

		// �萔�o�b�t�@�ݒ�
		pSM->CBWrite(NAME_TO(SHADER_POINT_LIGHT), &cbLight);
		pSM->BindCB(NAME_TO(SHADER_POINT_LIGHT));
	}

	pGBuff->SetUpTextures();
	// �`��
	pGBuff->DrawSpriteRender(CGBuffer::ETexture::COLOR, CScreen::GetWidth(), CScreen::GetHeight());
}

//========================================================
// �e�N�X�`���ݒ�
//========================================================
void CLightManager::SetUpTexture()const
{
	auto pTex = m_pRenderTarget->GetSRV();
	Application::Get()->GetDeviceContext()->PSSetShaderResources(Slot::ETextureSlot::TS_LIGHT_MAP, 1, &pTex);
}