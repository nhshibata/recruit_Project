//==========================================================
// [drawSystem.cpp]
//----------------------------------------------------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/09/11 視錘台当たり判定を調整
// 更新:2022/11/09 名前変更(DrawManager) -> (drawSystem)
// 更新:2022/12/15 インスタンシング描画対応
// 更新:2023/01/12 インスタンシング描画の実装に伴うバグ修正
//----------------------------------------------------------
// 描画管理
//==========================================================

//--- インクルード部
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>

#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CDrawSystem::CDrawSystem()
	:m_bIsSortNecessary(true)
{
	m_aPolygonList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();
	m_pDepthShadow = std::make_unique<CDepthShadow>();
	
#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
	m_bFrustum = true;
#endif // BUILD_MODE

}

//==========================================================
// デストラクタ
//==========================================================
CDrawSystem::~CDrawSystem()
{
	m_pDepthShadow.reset();
}

//==========================================================
// 初期化
//==========================================================
void CDrawSystem::Init()
{
	m_pDepthShadow->InitShader();
}

//==========================================================
// 登録
//==========================================================
int CDrawSystem::PolygonRegist(std::weak_ptr<CPolygonRenderer> render)
{
	int ret = CMapSystemBase::RegistToSystem(render.lock());
	// ポリゴン側に格納
	m_aPolygonList.push_back(render);
	return ret;
}

void CDrawSystem::SetInstanchingMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh)
{
	if (!m_aInstancingMeshMap.count(name))
		m_aInstancingMeshMap[name].pMesh = mesh;

	m_aInstancingMeshMap[name].aMtx.push_back(mtx);
}

//==========================================================
// 除外
//==========================================================
std::weak_ptr<CRenderer> CDrawSystem::ExecutSystem(int idx)
{
	auto release = IDToData(idx);

	// ポリゴンなら破棄されるｺﾝﾎﾟｰﾈﾝﾄを探し、整列リストから除外
	if (auto polygon = std::dynamic_pointer_cast<CPolygonRenderer>(release.lock()); polygon)
	{
		for (auto it = m_aPolygonList.begin(); it != m_aPolygonList.end(); ++it)
		{
			if (release.lock() == (*it).lock())
			{
				m_aPolygonList.erase(it);
				break;
			}
		}
	}
	return CMapSystemBase::ExecutSystem(idx);
}

//==========================================================
// 更新
//==========================================================
void CDrawSystem::Update()
{
	// CRenderer::Drawはboolを返す
	// trueであれば描画する
	// 描画数などの確認をするならばこれを活用する
	
	//--- レイヤーで並び替え
	// 追加されたときなど
	if (m_bIsSortNecessary)
	{
		Sort();
		m_bIsSortNecessary = false;
	}

	//--- BGの描画
	int UIIdx = 0;	// UIが何番目記憶するための変数
	for (int cnt = 0; cnt < m_aPolygonList.size(); ++cnt, ++UIIdx)
	{
		// ポインタ確認
		if (!m_aPolygonList[cnt].lock())
			continue;

		if (m_aPolygonList[cnt].lock()->GetZ() > static_cast<int>(CPolygonRenderer::EZValue::BG))
			break;

		// 描画可能な状態か確認
		if (!m_aPolygonList[cnt].lock()->IsActive() || !m_aPolygonList[cnt].lock()->IsVisible())
			continue;

		//--- 描画
		m_aPolygonList[cnt].lock()->Draw();
	}


	//--- 3D描画(Mesh,Model)
	for (auto & it : m_aIntMap)
	{
		auto render = it.second;

		// ポインタ確認
		if (!render.lock())
			continue;

		// 描画可能な状態か確認
		if (!render.lock()->IsActive() || !render.lock()->IsVisible())
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		//--- Meshｺﾝﾎﾟｰﾈﾝﾄ(および継承)か確認
		// カリングフラグも確認
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh)
		{	
			
			float fRadius = mesh->GetBSRadius();
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;
			
			//--- ｶﾒﾗに映るか判定
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius())==CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42 ,mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
#ifdef BUILD_MODE
				++m_nSkipCnt;
				if (m_bFrustum)
					continue;
#else
				continue;
#endif // _DEBUG
			}

		}

		//--- ここまできたら描画
		render.lock()->Draw();
	}

	//--- 3D描画
	// インスタンシング長いので関数化
	Draw3DShadow();	// 影描画
	Draw3D();		// 3D実描画

	//--- UI部分描画
	for (int cnt = UIIdx; cnt < m_aPolygonList.size(); ++cnt)
	{
		// ポインタ確認
		if (!m_aPolygonList[cnt].lock())
			continue;

		// 描画可能な状態か確認
		if (!m_aPolygonList[cnt].lock()->IsActive() || !m_aPolygonList[cnt].lock()->IsVisible())
			continue;

		//--- 描画
		m_aPolygonList[cnt].lock()->Draw();
	}

}

//==========================================================
// 整列
//==========================================================
void CDrawSystem::Sort()
{
	//--- 整列必要確認
	if (m_aPolygonList.size() <= 1)
		return;
	
	//--- ラムダ式で整列(昇順)
	std::sort(m_aPolygonList.begin(), m_aPolygonList.end(), [](auto const& s1, auto const& s2)->bool
	{
		return s1.lock()->GetZ() < s2.lock()->GetZ();
	});
}

//==========================================================
// 3Dインスタンシング影描画
// TODO:影描画フラグで影を描画するか決める仕組みがない
//==========================================================
void CDrawSystem::Draw3DShadow()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	//--- 深度値書き込み準備
	m_pDepthShadow->Begin();
	
	//--- 登録されたモデル名別に描画
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(intancingModel.first);
		// モデルが解放されていないか一応確認
		if (!model)
			continue;

		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly, false);
	}

	//--- Mesh
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.aMtx.size() == 0)// 一応確認
			continue;

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second.pMesh); bill != nullptr)
		{
			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			mesh.second.pMesh->DrawInstancing(mesh.second.aMtx, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			mesh.second.pMesh->DrawInstancing(mesh.second.aMtx, false);
		}
	}

	//--- 深度値書き込み終了
	m_pDepthShadow->End();

}

//==========================================================
// 3Dインスタンシング描画
//==========================================================
void CDrawSystem::Draw3D()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	CLight* pLight = CLight::GetMain();
	//pLight->SetDisable();			// ライティング無効
	//pDX->SetZBuffer(true);		// Z書き込み

	//--- 深度値ﾃｸｽﾁｬ設定
	// modelはt4
	m_pDepthShadow->SetUpTexture(4);

	//--- 登録されたモデル名別に描画
	//--- 不透明描画
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(intancingModel.first);
		// モデルが解放されていないか一応確認
		if (!model)
			continue;
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly);

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
		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(intancingModel.first);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eTransparentOnly);
	}
	
	pDX->SetZWrite(true);			// Z書き込み
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
	// クリア
	m_aInstancingModelMap.clear();


	//--- 深度値ﾃｸｽﾁｬ設定
	// meshはt1
	m_pDepthShadow->SetUpTexture(1);

	//--- メッシュインスタンシング描画
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.aMtx.size() == 0)// 一応確認
			continue;

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second.pMesh); bill != nullptr)
		{	
			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			mesh.second.pMesh->DrawInstancing(mesh.second.aMtx, true, tex, &bill->GetTextureMatrix());
		}
		else
		{
			mesh.second.pMesh->DrawInstancing(mesh.second.aMtx);
		}
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}

	// クリア
	m_aInstancingMeshMap.clear();

	//--- 設定を戻す
	pLight->SetEnable();		// ライティング有効
	//pDX->SetZBuffer(true);
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// αブレンディング無効

}


#ifdef BUILD_MODE

void CDrawSystem::ImGuiDebug()
{
	ImGui::Text("Renderer MAX : %d", m_aPolygonList.size());
	ImGui::Text(u8"Draw OK : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"Draw NO : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text("Instancing Num : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"Renderer Sort", &m_bIsSortNecessary);

	ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth()*0.3f, CScreen::GetHeight()*0.7f));
	ImGui::Begin("Shadow Depth");
	ImGui::Image(m_pDepthShadow->GetResource(), ImVec2(CScreen::GetWidth()*0.25f,CScreen::GetHeight()*0.25f));
	ImGui::End();

	//ImGui::Text("Resource/Model:%d", CModelManager::Get()->GetNameList().size());
	//ImGui::Text("Resource/Image:%d", CImageResourceManager::Get()->GetNameList().size());
	//ImGui::Text("Resource/Effekseer:%d", CEffekseer::Get()->GetNameList().size());

	//--- 次のフレーム用初期化
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}

#endif



#if POST_TEST

//==========================================================
// コンストラクタ
//==========================================================
CDrawSystemMap::CDrawSystemMap()
{

}

//==========================================================
// デストラクタ
//==========================================================
CDrawSystemMap::~CDrawSystemMap()
{

}

//==========================================================
// 更新
//==========================================================
void CDrawSystemMap::Update()
{
	// TODO:後で変える。実際の登録されたLayer順でなければ無駄が多い
	auto layer = static_cast<int>(CLayer::E_Layer::FOG);

	//--- レイヤー順に描画
	for (int cnt = 0; cnt < layer; ++cnt)
	{
		if (!m_aLayerMap.count(cnt))
			continue;
		
		//--- 前準備
		// 描画先変更や効果を掛けたい
		// 未作成:CPostProcessクラスがレイヤーに存在するか確認
		// 存在していれば効果を掛ける?

		// 描画処理
		m_aLayerMap[cnt].Update();
	}
}

#endif // POST_TEST
