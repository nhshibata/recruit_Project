//=========================================================
// [shaderAssets.cpp]
//---------------------------------------------------------
// �쐬:2023/02/09
//---------------------------------------------------------
// �e��V�F�[�_�[�f�ޗp
// �d�l��Astatic�֐�����
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Shader/shaderAssets.h>

#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>
#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/negative.h>
#include <GraphicsSystem/PostProcess/monochrome.h>
#include <GraphicsSystem/PostProcess/outline.h>

#include <ImGui/imgui.h>

#ifndef NAME_TO
#define NAME_TO(name)	#name	
#endif // NAME_TO

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//=========================================================
// ������
//=========================================================
HRESULT CShaderAssets::Init()
{
	HRESULT hr = S_OK;
	
	{
		// Toon�p�ǂݍ���
		auto imgMgr = Application::Get()->GetSystem<CAssetsManager>()->GetImageManager();
		imgMgr->Load(FORDER_DIR(Data/Texture/ramp.png));
	}
	//
	m_Rate.nega = 1.0f;
	m_Rate.mono = 0.0f;
	m_Rate.dummy = 0.0f;
	m_Rate.dummy_one = 0.0f;

	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_AssimpToon.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_AssimpToon", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_MeshToon.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_MeshToon", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_AssimpNega.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_AssimpNega", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_MeshNega.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_MeshNega", ps);
	}

	{
		ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
		hr = cb->Make(sizeof(SHADER_RATE), 7, CConstantBuffer::EType::Pixel);
		if (FAILED(hr))
			return hr;
		else
			pSM->SetCB(NAME_TO(SHADER_RATE), cb);
	}

	//--- �萔�o�b�t�@
	// ���z
	ConstantBufferSharedPtr sunCB = std::make_shared<CConstantBuffer>();
	hr = sunCB->Make(sizeof(SHADER_SUN), 6, CConstantBuffer::EType::Vertex);
	if (FAILED(hr))
		return hr;
	else
		pSM->SetCB(NAME_TO(SHADER_SUN), sunCB);

	// PS_DepthWrite
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_DepthWrite.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_DepthWrite", ps);
	}

	// VS_DepthWrite
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{	// �������ɂ̓Z�}���e�B�N�X���̐�����(�����)
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0,							D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	hr = vs->Make(FORDER_DIR(Data/shader/VS_DepthWrite.cso), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	else
		pSM->SetVS("VS_DepthWrite", vs);

	// PS_GBuffer
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_GBuffer.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_GBuffer", ps);
	}

	hr = CGaussianBlur::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CBloom::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CNegative::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CMonochrome::InitShader();
	if (FAILED(hr))
		return hr;
	hr = COutline::InitShader();
	if (FAILED(hr))
		return hr;

	//--- �R�[���o�b�N�ݒ�
	pSM->AddFunction("PS_AssimpToon", CShaderAssets::ToonSet);
	pSM->AddFunction("PS_MeshToon", CShaderAssets::ToonSet);
	pSM->AddFunction("PS_AssimpNega", CShaderAssets::NegaSet);
	pSM->AddFunction("PS_MeshNega", CShaderAssets::NegaSet);

	return hr;
}

//=========================================================
// ���
//=========================================================
void CShaderAssets::Uninit()
{
	
}

//=========================================================
// Toonø����ݒ�
//=========================================================
void CShaderAssets::ToonSet()
{
	auto pDC = Application::Get()->GetDeviceContext();

	auto imgMgr = Application::Get()->GetSystem<CAssetsManager>()->GetImageManager(); 
	auto pTex = imgMgr->GetResource(FORDER_DIR(Data/Texture/ramp.png))->GetSRV();
	pDC->PSSetShaderResources(5, 1, &pTex);
}

//=========================================================
// �l�K�p�萔�o�b�t�@ ��������&�o�C���h
//=========================================================
void CShaderAssets::NegaSet()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	pSM->CBWrite(NAME_TO(SHADER_RATE), &m_Rate);
	pSM->BindCB(NAME_TO(SHADER_RATE));
}


#ifdef BUILD_MODE

void CShaderAssets::ImGuiDebug()
{
	ImGui::DragFloat4("Nega:x Rate", (float*)&m_Rate, 0.1f, -5.0f, 5.0f);
}

#endif // BUILD_MODE