//=========================================================
// [depthShadow.cpp]
//---------------------------------------------------------
// 作成:2023/01/23
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/DirectX/depthShadow.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Transform/transform.h>

#include <Application/Application.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//=========================================================
// コンストラクタ
//=========================================================
CDepthShadow::CDepthShadow()
	:m_fFar(1000.0f * 0.7f), m_fNear(0.2f)
{
	m_vScreenSize.x = CScreen::GetWidth() * 5;
	m_vScreenSize.y = CScreen::GetHeight() * 5;
}

//=========================================================
// 初期化
//=========================================================
void CDepthShadow::InitShader()
{
	//--- メモリ確保
	m_pDepthStencil = std::make_shared<CDepthStencil>(
		(UINT)CScreen::GetWidth(),
		(UINT)CScreen::GetHeight(),
		true);
	m_pRenderTarget = std::make_shared<CRenderTarget>(
		DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, // 32bit = 4byteで書き込む場合のフォーマット
		(UINT)CScreen::GetWidth(),
		(UINT)CScreen::GetHeight());

	//--- 定数バッファ
	// 太陽
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	ConstantBufferSharedPtr sunCB = std::make_shared<CConstantBuffer>();
	if (FAILED(sunCB->Make(sizeof(SHADER_SUN), Slot::CB_SUN_VP, CConstantBuffer::EType::Vertex)))
	{

	}
	else
		sm->SetCB(NAME_TO(SHADER_SUN), sunCB);

	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	if (FAILED(ps->Make(CPixelName::GetCSO(CPixelName::szDepthWrite))))
	{

	}
	else
		sm->SetPS(CPixelName::szDepthWrite,ps);

	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{	// 第二引数にはセマンティクス名の数字を(あれば)
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0,							D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	if (FAILED(vs->Make(CVertexName::GetCSO(CVertexName::szDepthWrite), layout, _countof(layout))))
	{

	}
	else
		sm->SetVS(CVertexName::szDepthWrite, vs);
}

//=========================================================
// デスクトラクタ
//=========================================================
CDepthShadow::~CDepthShadow()
{
	m_pDepthStencil.reset();
	m_pRenderTarget.reset();
}

//=========================================================
// 描画前
//=========================================================
void CDepthShadow::Begin()
{
	//--- 描画クリア
	// Shader内でZ値を確認するため、奥行きとして1を設定
	float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };
	m_pDepthStencil->Clear();
	m_pRenderTarget->Clear(clearColor);
	
	//--- 描画先切替
	ID3D11RenderTargetView* view[] = { m_pRenderTarget->GetView() };
	Application::Get()->GetSystem<CDXDevice>()->SwitchRender(*view, m_pDepthStencil->GetView());

	//--- 定数バッファ書き込み
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// ps,vs設定
	sm->BindPS(CPixelName::szDepthWrite);
	sm->BindVS(CVertexName::szDepthWrite);

	//--- ライト
	CDirectionalLight* light = dynamic_cast<CDirectionalLight*>(CLight::GetMain());
	if (!light)
		return;

	//auto sunPos = light->Transform()->GetPos();
	auto dir = light->GetDir();

	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(
		//DirectX::XMVectorSet(sunPos.x, sunPos.y, sunPos.z, 0),
		DirectX::XMVectorSet(50, 500, -3, 0),
		DirectX::XMVectorSet(dir.x, dir.y, dir.z, 0),
		DirectX::XMVectorSet(0, 1, 0, 0)
	);
	// 平行投影	
	DirectX::XMMATRIX sunProj = DirectX::XMMatrixOrthographicLH(
		m_vScreenSize.x, m_vScreenSize.y, m_fNear, m_fFar
		//5.0f, 5.0f * (9.0f / 16.0f), 0.2f, 1000.0f
	);
	SHADER_SUN sunMat;
	DirectX::XMStoreFloat4x4(&sunMat.sunView, DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&sunMat.sunProj, DirectX::XMMatrixTranspose(sunProj));
	// 1,2番目を渡す
	sm->CBWrite(NAME_TO(SHADER_SUN), &sunMat);
	sm->BindCB(NAME_TO(SHADER_SUN));

}

//=========================================================
// 描画後
//=========================================================
void CDepthShadow::End()
{
	// 元に戻す
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SwitchRender(pDX->GetRenderTargetView(), pDX->GetDepthStencilView());

	
}

//=========================================================
// 深度値ﾃｸｽﾁｬを設定する
//=========================================================
void CDepthShadow::SetUpTexture(UINT slot)
{
	// model4?
	auto pTex = m_pRenderTarget->GetSRV();
	Application::Get()->GetSystem<CDXDevice>()->GetDeviceContext()->PSSetShaderResources(slot, 1, &pTex);
}


#if BUILD_MODE

void CDepthShadow::ImGuiDebug()
{

	Debug::SetTextAndAligned("Near");
	ImGui::DragFloat("##Near", (float*)&m_fNear, 0.1f, 0.1f);

	Debug::SetTextAndAligned("Far");
	ImGui::DragFloat("##Far", (float*)&m_fFar, 0.1f, 0.2f);
	
	Debug::SetTextAndAligned("Projection ScreenSize");
	ImGui::DragFloat2("##Projection Screen", (float*)&m_vScreenSize, 10.0f);
	
	ImGui::Image(GetResource(), ImVec2(CScreen::GetWidth()*0.25f, CScreen::GetHeight()*0.25f));

}

#endif // BUILD_MODE