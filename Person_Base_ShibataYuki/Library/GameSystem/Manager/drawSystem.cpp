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
// 描画対象の確認
// Baseとほぼ同じだが、Layerが追加され、volumeへ格納している
//==========================================================
void CDrawSystem::CheckRenderedObjectsIn3D()
{
	// キャッシュのクリア
	m_VolumeMgr.ResetRendererCash();

	//--- 3D描画(Mesh,Model)
	for (auto & it : m_aIntMap)
	{
		auto render = it.second.lock();

		// ポインタ確認
		if (!render)
			continue;

		// 描画可能な状態か確認
		if (!render->IsActive() || !render->IsVisible())
			continue;

		// layer確認
		if (!CCamera::GetMain()->IsMask(render->GetLayer()))
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render->GetName();
#endif // _DEBUG

		//--- Meshｺﾝﾎﾟｰﾈﾝﾄ(および継承)か確認
		// カリングフラグも確認
		if (auto mesh = render->BaseToDerived<CMeshRenderer>().get(); mesh)
		{

			float fRadius = mesh->GetBSRadius();
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;

			//--- ｶﾒﾗに映るか判定
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius())==CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42, mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
				render->GetOwner()->CameraTest(false);
#ifdef BUILD_MODE
				++m_nSkipCnt;
				if (m_bFrustum)
					continue;
#else
				continue;
#endif // _DEBUG
			}

		}

		// ここまできたら描画
		render->Draw();
		render->GetOwner()->CameraTest(true);

		// 所属LayerとIDを格納
		m_VolumeMgr.AddRendererCash(CLayer::NumberToBit(render->GetLayer()), it.first);
	}
}

//==========================================================
// 3Dインスタンシング描画
//==========================================================
void CDrawSystem::GBufferDraw(const bool bGbuffer, std::function<bool(int)> func)
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	LPCSTR aVSName[] = {
		"VS_Assimp",
		"VS_Mesh",
	};
	CLight* pLight = CLight::GetMain();

	if (!bGbuffer)
	{
		//pLight->SetDisable();
		pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	}

	//--- 登録されたモデル名別に描画
	//--- 不透明描画
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = TextSplitToNamePSVS(intancingModel.first);
		if (aName.IsError())
			continue;

		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(aName.strName);
		// モデルが解放されていないか一応確認
		if (!model)
			continue;

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[0]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}

	if (!bGbuffer)
	{
		// ライティング有効
		pLight->SetEnable();

		//--- 半透明部分描画
		pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
		pDX->SetZWrite(false);// Z書き込み
	}

	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = TextSplitToNamePSVS(intancingModel.first);
		if (aName.IsError())
			continue;

		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(aName.strName);

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[0]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}

			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eTransparentOnly, false);
	}

	if (!bGbuffer)
	{
		pDX->SetZWrite(true);	// Z書き込み
		pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
	}

	//--- メッシュインスタンシング描画
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		if (meshObj.second.aID.size() == 0)// 一応確認
			continue;

		auto aName = TextSplitToNamePSVS(meshObj.first);
		if (aName.IsError())
			continue;

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[1]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : meshObj.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(aName.strName);
			auto tex = image ? image->GetSRV() : NULL;
			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			meshObj.second.pMesh->DrawInstancing(data, false);
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

#if BUILD_MODE	// ImGui表示中はDebugCameraがMainなので、hierarchyを探索
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
	{
		CDrawSystemBase::Draw3D();
		return;
	}
#else

	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
	{
		CDrawSystemBase::Draw3D();
		return;
	}

#endif // BUILD_MODE

	// ｶﾒﾗを順に描画
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto aCamera = pCamera->GetStackCameras();
	int idxCnt = 0;
	std::vector<ID3D11ShaderResourceView*> aEffectTex;

	do
	{
		//--- GBufferが必要なLayerを取得
		const int layerBit = m_VolumeMgr.GetBit(pCamera->GetMask());
		// ラムダ式
		// 描画条件設定を定義
		auto layerChaeck = [=](int id)->bool {
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			return (CLayer::NumberToBit(mesh->GetLayer()) & layerBit);
		};
		auto layerChaeckCamera = [=](int id)->bool {
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			return (CLayer::NumberToBit(mesh->GetLayer()) & pCamera->GetMask());
		};

		//--- GBuffer描画
		// レンダーターゲットの設定
		pCamera->GetGBuffer()->SetUpMultiRenderTarget();
		// 3D描画
		GBufferDraw(true, layerChaeck);

		// Sceneに描画先を変更
		pDX->SwitchRender(pDX->GetRenderTargetView(), pDX->GetDepthStencilView());
		// 3D描画
		pCamera->GetGBuffer()->SetSRV(CGBuffer::ETexture::DEPTH);
		GBufferDraw(false, layerChaeckCamera);

		// 各種ﾃｸｽﾁｬの設定
		// NOTE:ここで呼び出しても何故かﾃｸｽﾁｬ設定されない
		//pCamera->GetGBuffer()->SetUpTextures();
		// ﾃｸｽﾁｬのｺﾋﾟｰ
		// NOTE:スクリーン用のTextureが上手く設定されていない?
		//pCamera->GetGBuffer()->CopyTexture();

		//--- ポストプロセス
		auto aVolume = m_VolumeMgr.GetVolume(pCamera->GetMask());
		for (auto & vol : aVolume)
		{
			if (1)
			{
				auto aID = vol->GetRenderCash();
				// ラムダ式
				auto idChaeck = [=](int id)->bool {
					for (auto & no : aID)
					{
						if (no == id)
							return true;
					}
					return false;
				};
				//--- GBuffer描画
				// レンダーターゲットの設定
				pCamera->GetGBuffer()->SetUpColorRenderTarget();
				//pCamera->GetGBuffer()->SetUpMultiRenderTarget();
				// 3D描画
				GBufferDraw(true, idChaeck);
			}
			vol->GetEffect()->DrawSprite(pCamera->GetGBuffer());
			aEffectTex.push_back(vol->GetEffect()->GetResource());
		}

		// 先頭から順にｶﾒﾗを設定
		if (idxCnt < aCamera.size())
		{
			pCamera = aCamera[idxCnt].lock();
			idxCnt++;
		}
		else
		{
			pCamera.reset();
		}

	} while (pCamera);

	//--- レンダーターゲットをデフォルトに戻す
	pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());
	//pDX->SwitchRender(pDX->GetRenderTargetView(), nullptr);
	
	// volumeが一切ないので処理しない
	if (aEffectTex.size() == 0)
		return;

	//--- ﾃｸｽﾁｬ合成
	//--- 描画設定
	pDX->SetZBuffer(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(CScreen::GetSize());
	CPolygon::SetPos(XMFLOAT2(0,0));
	CPolygon::SetFrameSize(XMFLOAT2(1,1));
	CPolygon::SetUV(XMFLOAT2(0,0));
	CPolygon::SetAngle(0);
	
	CPolygon::SetTexture(aEffectTex[0]);
	//CPolygon::Draw(pDX->GetDeviceContext());

	// 加算合成必須
	pDX->SetBlendState(EBlendState::BS_ADDITIVE);

	// ポリゴン描画
	for (int cnt = 0; cnt < aEffectTex.size(); ++cnt)
	{
		CPolygon::SetTexture(aEffectTex[cnt]);
		CPolygon::Draw(pDX->GetDeviceContext());
	}

	CPolygon::SetSize(XMFLOAT2(1,1));
	pDX->SetBlendState(EBlendState::BS_NONE);
	pDX->SetZBuffer(true);

	pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());

}


#ifdef BUILD_MODE

//void CDrawSystem::ImGuiDebug()
//{
//	
//}

#endif

