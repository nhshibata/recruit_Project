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
	//ImGui::DragFloat4("Nega:x Rate", (float*)&m_Rate, 0.1f, -5.0f, 5.0f);
}

#endif // BUILD_MODE