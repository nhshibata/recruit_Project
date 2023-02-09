//=========================================================
// [shaderAssets.cpp]
//---------------------------------------------------------
// 作成:2023/02/09
//---------------------------------------------------------
// 各種シェーダー素材用
// 仕様上、static関数多数
//=========================================================

//--- インクルード部
#include <GameSystem/Shader/shaderAssets.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <Application/Application.h>

#include <ImGui/imgui.h>


#ifndef NAME_TO
#define NAME_TO(name)	#name	
#endif // NAME_TO

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//=========================================================
// 初期化
//=========================================================
void CShaderAssets::Init()
{
	// Toon用読み込み
	m_pToonResource.Load(FORDER_DIR(Data/Texture/ramp.png));

	//
	m_Rate.nega = 1.0f;
	m_Rate.mono = 0.0f;
	m_Rate.dummy = 0.0f;
	m_Rate.dummy_one = 0.0f;

	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		if (FAILED(ps->Make(CSO_PATH(PS_AssimpToon.cso))))
		{

		}
		else
			pSM->SetPS("PS_AssimpToon", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		if (FAILED(ps->Make(CSO_PATH(PS_MeshToon.cso))))
		{

		}
		else
			pSM->SetPS("PS_MeshToon", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		if (FAILED(ps->Make(CSO_PATH(PS_AssimpNega.cso))))
		{

		}
		else
			pSM->SetPS("PS_AssimpNega", ps);
	}
	
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		if (FAILED(ps->Make(CSO_PATH(PS_MeshNega.cso))))
		{

		}
		else
			pSM->SetPS("PS_MeshNega", ps);
	}

	{
		ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
		cb->Make(sizeof(SHADER_RATE), 7, CConstantBuffer::EType::Pixel);
		pSM->SetCB(NAME_TO(SHADER_RATE), cb);
	}

	//--- コールバック設定
	pSM->AddFunction("PS_AssimpToon", CShaderAssets::ToonSet);
	pSM->AddFunction("PS_MeshToon", CShaderAssets::ToonSet);
	pSM->AddFunction("PS_AssimpNega", CShaderAssets::NegaSet);
	pSM->AddFunction("PS_MeshNega", CShaderAssets::NegaSet);
}

//=========================================================
// 解放
//=========================================================
void CShaderAssets::Uninit()
{
	m_pToonResource.Unload();
}

//=========================================================
// Toonﾃｸｽﾁｬ設定
//=========================================================
void CShaderAssets::ToonSet()
{
	auto pDC = Application::Get()->GetDeviceContext();

	auto pTex = m_pToonResource.GetSRV();
	pDC->PSSetShaderResources(5, 1, &pTex);
}

//=========================================================
// ネガ用定数バッファ 書き込み&バインド
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