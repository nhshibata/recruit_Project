//==========================================================
// [drawSystem.cpp]
// 派生クラス
//---------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/09/11 視錘台当たり判定を調整
// 更新:2022/11/09 クラス名変更(DrawManager) -> (drawSystem)
//==========================================================

//--- インクルード部
#include <algorithm>
#include <ImGui/imgui.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/camera.h>

#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>


using namespace MySpace::Game;

CDrawSystem::CDrawSystem()
	:m_bIsSortNecessary(true), m_bFrustum(true)
{
	m_pDrawSortList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMesh.clear();
	
#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
#endif // BUILD_MODE


}
CDrawSystem::~CDrawSystem()
{
	// 解放処理
	CImageResourceManager::Get()->Uninit();
	CEffekseer::Get()->Uninit();
	CModelManager::Get()->Uninit();
}
void CDrawSystem::Update()
{
	// CRenderer::Drawはboolを返す
	// trueであれば描画する
	// 描画数などの確認をするならばこれを活用する
	
	//--- レイヤーで並び替え
	// 追加されたときなど
	if (m_bIsSortNecessary)
	{
		// 整列
		//m_pObjectList.back().lock()->;
		std::sort(m_pDrawSortList.begin(), m_pDrawSortList.end(), [](auto const& s1, auto const& s2)->bool
		{
			return s1.lock()->GetLayer() < s2.lock()->GetLayer();
		});

		m_bIsSortNecessary = false;
	}
	
	//--- 描画
	for (auto & render : m_pDrawSortList)
	{
		// ポインタ確認
		if (!render.lock())
			continue;

		// 描画可能な状態か確認
		if (!render.lock()->IsActive())
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		//--- Meshｺﾝﾎﾟｰﾈﾝﾄ(および継承)か確認
		// カリングフラグがONかも確認
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh && m_bFrustum)
		{	
			float fRadius = mesh->GetBSRadius();
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;
			
			// ｶﾒﾗに映るか判定
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius()) == CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42 ,mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
#ifdef BUILD_MODE
				++m_nSkipCnt;
#endif // _DEBUG
				continue;
			}
		}

		//--- ここまできたら描画
		render.lock()->Draw();
	}

	//--- インスタンシング描画
	CLight* pLight = CLight::Get();
	pLight->SetDisable(false);			// ライティング無効
	//--- 登録された名前別に描画
	const int MAX_INSTANCING = 100;
	for (auto & draw : m_aInstancingModelMap)
	{
		auto model = CModelManager::Get()->GetModel(draw.first);
		//--- 定数以上の時
		// インスタンシングには定数分しか領域を確保していないため、その値以上の場合は一旦区切る
		if (draw.second.size() >= MAX_INSTANCING)
		{
			std::vector<DirectX::XMFLOAT4X4> inData;
			for (int idxCnt = 0; idxCnt < static_cast<int>(draw.second.size()); ++idxCnt)
			{
				int idx = idxCnt % MAX_INSTANCING;
				if (idxCnt != 0 && idx == MAX_INSTANCING - 1)
				{
					model->DrawInstancing(CDXDevice::Get()->GetDeviceContext(), inData);
					inData.clear();
					continue;
				}
				inData[idx] = draw.second[idx];
			}
			// 次のモデルへ
			draw.second.clear();
			continue;
		}

		model->DrawInstancing(CDXDevice::Get()->GetDeviceContext(), draw.second);
		draw.second.clear();

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG

	}
	// クリア
	m_aInstancingModelMap.clear();

	//--- メッシュインスタンシング描画
	for (auto & mesh : m_aInstancingMesh)
	{
		if (mesh.second.size() == 0)// 一応確認
			continue;
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG

		// ビルボードか確認
		CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second[0]);
		if (bill)
		{
			auto image = CImageResourceManager::Get()->GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			CMesh::DrawInstancing(mesh.second, tex, &bill->GetTextureMatrix());
		}
		else
		{
			CMesh::DrawInstancing(mesh.second);
		}
	}
	m_aInstancingMesh.clear();	// クリア

	//--- 設定を戻す
	pLight->SetEnable();		// ライティング有効
	CDXDevice::Get()->SetZBuffer(true);			
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// αブレンディング無効


#if 0
	if (m_bIsSort)
	{
		std::sort(m_ObjectList.begin(), m_ObjectList.end(), [](auto const& s1, auto const& s2)->bool
		{
			/*if (!s1.lock()->IsActive())
				activ.push_back(s1.lock());*/
			auto name = s1.lock()->GetName();
			if (s1.lock() && s2.lock())
				return false;
			//return s1.lock()->GetOwner() > s2.lock()->GetOwner();
			return s1.lock()->GetOwner()->GetLayer() < s2.lock()->GetOwner()->GetLayer();
		});
		m_bIsSort = false;
	}

	for (auto & render : m_ObjectList)
	{
		// ポインタ確認
		if(!render.lock())
		{
			continue;		// null
		}

		// 描画可能な状態か確認
		if (!render.lock()->IsActive())
			continue;

#ifdef _DEBUG
		++cnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		// Meshｺﾝﾎﾟｰﾈﾝﾄか確認
		// ｶﾒﾗに映るか判定
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh)
		{
			if (!CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(), mesh->GetBSRadius()))
			{
				continue;
			}
		}

		// 描画
		render.lock()->Draw();
	}
#endif // 0

}

#ifdef BUILD_MODE

void CDrawSystem::ImGuiDebug()
{
	ImGui::Text(u8"描画リスト数 : %d", m_pDrawSortList.size());
	ImGui::Text(u8"描画OK数 : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"描画スキップ数 : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text(u8"インスタンシング数 : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"描画ソートON", &m_bIsSortNecessary);

	ImGui::Text("Resource/Model:%d", CModelManager::Get()->GetNameList().size());
	ImGui::Text("Resource/Image:%d", CImageResourceManager::Get()->GetNameList().size());
	ImGui::Text("Resource/Effekseer:%d", CEffekseer::Get()->GetNameList().size());

	//--- 次のフレーム用初期化
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}
#endif