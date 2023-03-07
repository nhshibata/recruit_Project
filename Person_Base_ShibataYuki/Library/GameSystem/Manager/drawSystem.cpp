//==========================================================
// [drawSystem.cpp]
//----------------------------------------------------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/09/11 視錘台当たり判定を調整
// 更新:2022/11/09 名前変更(DrawManager) -> (drawSystem)
// 更新:2022/12/15 インスタンシング描画対応
// 更新:2023/01/12 インスタンシング描画の実装に伴うバグ修正
// 更新:2023/02/08 シェーダー分け
//----------------------------------------------------------
// 描画管理
//==========================================================

//--- インクルード部
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/stackCamera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Shader/shaderAssets.h>

#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CDrawSystem::CDrawSystem()
{
	

}

//==========================================================
// デストラクタ
//==========================================================
CDrawSystem::~CDrawSystem()
{
	
}

//==========================================================
// 更新
//==========================================================
void CDrawSystem::Update()
{
	CDrawSystemBase::Update();
}

//==========================================================
// 3Dインスタンシング描画
//==========================================================
void CDrawSystem::GBufferDraw()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	LPCSTR vsName[] = {
		"VS_Assimp",
		"VS_Mesh",
	};

	pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	CLight* pLight = CLight::GetMain();
	//pLight->SetDisable();			// ライティング無効
	//pDX->SetZBuffer(true);		// Z書き込み

	//--- 登録されたモデル名別に描画
	//--- 不透明描画
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(intancingModel.first);
		if (aName.size() != 3)
			continue;

		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);
		// モデルが解放されていないか一応確認
		if (!model)
			continue;
		// shaderBind
		pSM->CallBackFuncAndBind(std::string(), vsName[0]);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	pLight->SetEnable();			// ライティング有効


	//--- 半透明部分描画
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);			// Z書き込み

	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(intancingModel.first);
		if (aName.size() != 3)
			continue;

		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);

		// shaderBind
		pSM->CallBackFuncAndBind(std::string(), vsName[0]);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eTransparentOnly, false);
	}

	pDX->SetZWrite(true);	// Z書き込み
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	//--- メッシュインスタンシング描画
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.aData.size() == 0)// 一応確認
			continue;

		std::vector<std::string> aName = GetPSVSName(mesh.first);
		if (aName.size() != 3)
			continue;

		// shaderBind
		pSM->CallBackFuncAndBind(std::string(), vsName[1]);

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second.pMesh); bill != nullptr)
		{
			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(aName[0]);
			auto tex = image ? image->GetSRV() : NULL;
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false);
		}
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG

	}

	//--- 設定を戻す
	pLight->SetEnable();		// ライティング有効
	//pDX->SetZBuffer(true);
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// αブレンディング無効

}

//=========================================================
// Updateから呼び出される関数のオーバーライド
//=========================================================
void CDrawSystem::Draw3D()
{
#if BUILD_MODE
	// ｶﾒﾗを見つける
	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
	{
		auto all = CCamera::GetAllCamera();
		for (auto & cam : all)
		{
			pCamera = cam->BaseToDerived<CStackCamera>();
			if (pCamera)
				break;
		}
	}
	if (!pCamera)
		return;
#else

	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
		return;

#endif // BUILD_MODE

	// ｶﾒﾗを順に描画
	auto aCamera = pCamera->GetStackCamera();
	int idx = 0;
	std::vector<ID3D11ShaderResourceView*> aTex;

	do
	{
		//--- GBuffer描画
		// レンダーターゲットの設定
		pCamera->GetGBuffer()->SetUpMultiRenderTarget();

		// 3D描画
		GBufferDraw();

		// 各種ﾃｸｽﾁｬの設定
		// MEMO:ここで呼び出すても何故かﾃｸｽﾁｬ設定されない
		pCamera->GetGBuffer()->SetUpTextures();

		//--- ポストプロセス
		auto aVolume = m_VolumeMgr.GetVolume(pCamera->GetMask());
		for (auto & vol : aVolume)
		{
			vol->GetEffect()->DrawSprite(pCamera->GetGBuffer());
			aTex.push_back(vol->GetEffect()->GetResource());
		}

		// 先頭から順にｶﾒﾗを設定
		if (idx < aCamera.size())
		{
			pCamera = aCamera[idx].lock();
			idx++;
		}
		else
			pCamera.reset();

	} while (pCamera);

	//--- ﾃｸｽﾁｬ合成
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	
	//pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());
	pDX->SwitchRender(pDX->GetRenderTargetView(), nullptr);
	
	if (aTex.size() == 0)
		return;

	//--- 描画設定
	pDX->SetZBuffer(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(CScreen::GetSize());
	CPolygon::SetPos(XMFLOAT2(0,0));
	CPolygon::SetFrameSize(XMFLOAT2(1,1));
	CPolygon::SetUV(XMFLOAT2(0,0));
	CPolygon::SetAngle(0);
	
	CPolygon::SetTexture(aTex[0]);

	pDX->SetBlendState(EBlendState::BS_ADDITIVE);
	for (int cnt = 1; cnt < aTex.size(); ++cnt)
	{
		CPolygon::SetTexture(aTex[cnt]);
		CPolygon::Draw(pDX->GetDeviceContext());
	}

	CPolygon::SetSize(XMFLOAT2(1,1));
	pDX->SetBlendState(EBlendState::BS_NONE);
	pDX->SetZBuffer(true);

}


#ifdef BUILD_MODE

//void CDrawSystem::ImGuiDebug()
//{
//	
//}

void CDrawSystem::SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh)
{
	if (!m_aDebugMeshMap.count(name))
		m_aDebugMeshMap[name].pMesh = mesh;

	m_aDebugMeshMap[name].mtx.push_back(mtx);
}

#endif

