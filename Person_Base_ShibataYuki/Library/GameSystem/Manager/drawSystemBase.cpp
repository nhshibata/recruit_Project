//=========================================================
// [drawSystem.cpp]
// 派生クラス
//---------------------------------------------------------
// 
//---------------------------------------------------------
// drawSystemが長くなったのと、
// 旧システムを大きく変える必要がないと思い、基底クラスを挟む
//=========================================================

//--- インクルード部
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystemBase.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/stackCamera.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Shader/shaderAssets.h>

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
CDrawSystemBase::CDrawSystemBase()
	:m_bIsSortNecessary(true)
{
	m_aPolygonList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();
	m_pDepthShadow = std::make_unique<CDepthShadow>();

#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
	m_bFrustum = true;
	m_bShadowView = true;
#endif // BUILD_MODE

}

//==========================================================
// デストラクタ
//==========================================================
CDrawSystemBase::~CDrawSystemBase()
{
	m_pDepthShadow.reset();

	CShaderAssets::Uninit();
}

//==========================================================
// 初期化
//==========================================================
void CDrawSystemBase::Init()
{
	m_pDepthShadow->InitShader();

	CShaderAssets::Init();
}

//==========================================================
// 更新
//==========================================================
void CDrawSystemBase::Update()
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

	// 描画対象の確認
	CheckRenderedObjectsIn3D();

	//--- 3D描画
	// インスタンシング長いので関数化
	Draw3D();		// 3D実描画

	// クリア
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();

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

#ifdef BUILD_MODE
	//--- 当たり判定メッシュ描画
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	for (auto & name : m_aDebugMeshMap)
	{
		std::vector<XMFLOAT4X4> inMtx;
		for (auto & mesh : name.second)
		{
			inMtx.push_back(mesh->GetWorld());
		}
		name.second[0]->DrawInstancing(inMtx, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
#endif // BUILD_MODE

}

//==========================================================
// 描画対象の確認
//==========================================================
void CDrawSystemBase::CheckRenderedObjectsIn3D()
{
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

		// layer確認
		if (!CCamera::GetMain()->IsMask(render.lock()->GetLayer()))
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
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42, mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
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

		// ここまできたら描画
		render.lock()->Draw();
	}
}

//==========================================================
// 登録
//==========================================================
int CDrawSystemBase::PolygonRegist(std::weak_ptr<CPolygonRenderer> render)
{
	int ret = CMapSystemBase::RegistToSystem(render.lock());
	// ポリゴン側に格納
	m_aPolygonList.push_back(render);
	return ret;
}

//==========================================================
// 登録 
// インスタンシング描画のために情報を格納する
//==========================================================
void CDrawSystemBase::SetInstanchingModel(std::string name, std::string ps, std::string vs, const int id)
{
	const auto pushName = name + "#" + ps + "#" + vs;
	m_aInstancingModelMap[pushName].aID.push_back(id);
}

//==========================================================
// 登録
//==========================================================
void CDrawSystemBase::SetInstanchingMesh(std::string name, std::string ps, std::string vs, const int id, CMesh* mesh)
{
	const auto pushName = name + "#" + ps + "#" + vs;
	if (!m_aInstancingMeshMap.count(pushName))
		m_aInstancingMeshMap[pushName].pMesh = mesh;

	m_aInstancingMeshMap[pushName].aID.push_back(id);
}

//==========================================================
// リスト取得
//==========================================================
std::vector<std::weak_ptr<CRenderer>> CDrawSystemBase::GetList()
{
	std::vector<std::weak_ptr<CRenderer>> ret;
	for (auto & it : m_aIntMap)
	{
		ret.push_back(it.second);
	}
	return ret;
}

//=========================================================
// map格納時の名前を分割し、返却
// 0:登録名 1:PS 2:VS
// FIXME:ラムダ式に変えてもいいかも
//=========================================================
const std::vector<std::string> CDrawSystemBase::GetPSVSName(const std::string name)
{
	std::vector<std::string> ret;
	// ps座標取得
	auto startIndex = name.find("#");
	// 名前格納
	ret.push_back(name.substr(0, startIndex));
	startIndex += 1;
	// vs座標取得
	auto endIndex = name.find("#", startIndex);
	//--- 文字切り出し格納
	ret.push_back(name.substr(startIndex, endIndex - startIndex));
	endIndex += 1;
	ret.push_back(name.substr(endIndex, name.size() - (startIndex + endIndex)));
	return ret;
}

//==========================================================
// 除外
//==========================================================
std::weak_ptr<CRenderer> CDrawSystemBase::ExecutSystem(int idx)
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
// 整列
//==========================================================
void CDrawSystemBase::Sort()
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
void CDrawSystemBase::Draw3DShadow()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	//--- 深度値書き込み準備
	m_pDepthShadow->Begin();

	//--- 登録されたモデル名別に描画
	for (auto & modelObj : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(modelObj.first);
		if (aName.size() != 3)
			continue;

		//--- 描画するモデルの取得
		auto pModel = pAssets->GetModelManager()->GetModel(aName[0]);
		// モデルが解放されていないか一応確認
		if (!pModel)
			continue;

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : modelObj.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		pModel->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);
	}

	//--- Mesh
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		if (meshObj.second.aID.size() == 0)// 一応確認
			continue;

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			auto aName = GetPSVSName(meshObj.first);
			if (aName.size() != 3)
				continue;

			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(aName[0]);
			auto tex = image ? image->GetSRV() : NULL;

			//--- インスタンシングに必要なデータ格納
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			//--- インスタンシングに必要なデータ格納
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

			meshObj.second.pMesh->DrawInstancing(data, false);
		}
	}

	//--- 深度値書き込み終了
	m_pDepthShadow->End();

	//--- 深度値ﾃｸｽﾁｬ設定
	// t4
	m_pDepthShadow->SetUpTexture(4);
}

//==========================================================
// 3Dインスタンシング描画
//==========================================================
void CDrawSystemBase::Draw3D()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// 影描画
	Draw3DShadow();	

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

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);
		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	pLight->SetEnable();			// ライティング有効


	//--- 半透明部分描画
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);			// Z書き込み

	for (auto & modelObj : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(modelObj.first);
		if (aName.size() != 3)
			continue;

		//--- 描画するモデルの取得
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);

		//--- インスタンシングに必要なデータ格納
		std::vector<RENDER_DATA> data;
		for (auto & id : modelObj.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);
		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eTransparentOnly, false);
	}

	pDX->SetZWrite(true);			// Z書き込み
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	//--- メッシュインスタンシング描画
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		if (meshObj.second.aID.size() == 0)// 一応確認
			continue;

		std::vector<std::string> aName = GetPSVSName(meshObj.first);
		if (aName.size() != 3)
			continue;

		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);

		// ビルボードか確認
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			// ﾃｸｽﾁｬ設定
			auto image = pAssets->GetImageManager()->GetResource(aName[0]);
			auto tex = image ? image->GetSRV() : NULL;
			
			//--- インスタンシングに必要なデータ格納
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			//--- インスタンシングに必要なデータ格納
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

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


#if BUILD_MODE

void CDrawSystemBase::ImGuiDebug()
{
	ImGui::Text("Renderer MAX : %d", m_aPolygonList.size());
	ImGui::Text(u8"Draw OK : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"Draw Skip : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text("Instancing Num : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"Renderer Sort", &m_bIsSortNecessary);

	//--- shadow レンダーターゲット表示
	if (m_pDepthShadow)
	{
		ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth()*0.3f, CScreen::GetHeight()*0.7f), ImGuiCond_::ImGuiCond_Once);
		if (ImGui::Begin("Shadow Depth", (bool*)m_bShadowView))
		{
			ImGui::Image(m_pDepthShadow->GetResource(), ImVec2(CScreen::GetWidth()*0.25f, CScreen::GetHeight()*0.25f));
			ImGui::End();
		}
	}

	CShaderAssets::ImGuiDebug();

	//--- 次のフレーム用初期化
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}

void CDrawSystemBase::SetDebugMesh(std::string name, CMesh* mesh)
{
	m_aDebugMeshMap[name].push_back(mesh);
}

void CDrawSystemBase::ReleaseDebugMesh(CMesh* pMesh)
{
	for (auto & name : m_aDebugMeshMap)
	{
		for (auto & mesh : name.second)
		{
			// 同一のメッシュﾎﾟｲﾝﾀを見つけたので除外
			if (pMesh == mesh)
			{
				auto it = std::find(name.second.begin(), name.second.end(), mesh);
				name.second.erase(it);
				return;
			}
		}
	}
}

#endif // BUILD_MODE
