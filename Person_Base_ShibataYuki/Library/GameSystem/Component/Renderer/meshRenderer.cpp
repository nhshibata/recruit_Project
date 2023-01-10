//=========================================================
// [renderer.cpp] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CMeshRenderer::CMeshRenderer() 
	:m_vCenter(0, 0, 0), m_bLightEnable(true), m_fBSRadius(1),m_nStaticMode(static_cast<int>(EStaticMode::NONE))
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CMeshRenderer::CMeshRenderer(std::shared_ptr<CGameObject> owner)
	: CRenderer(owner), m_vCenter(0,0,0), m_bLightEnable(true), m_fBSRadius(1), m_nStaticMode(static_cast<int>(EStaticMode::NONE))
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CMeshRenderer::~CMeshRenderer()
{
}

//==========================================================
// 初期化
//==========================================================
void CMeshRenderer::Init()
{
	// 描画登録
	CRenderer::Init();
}

//==========================================================
// 描画
//==========================================================
bool CMeshRenderer::Draw()
{
	return CRenderer::Draw();
}

//==========================================================
// 中心座標
//==========================================================
Vector3 CMeshRenderer::GetCenter(int n) 
{
	return m_vCenter + Transform()->GetPos(); 
}

//==========================================================
// バウンディングスフィア取得
//==========================================================
float CMeshRenderer::GetBSRadius()
{
	return m_fBSRadius * Transform()->GetScale().GetLargeValue(); 
};

//==========================================================
// インスタンシング設定
//==========================================================
void CMeshRenderer::SetInstancing(CMesh* mesh, std::string name)
{
	auto sys = SceneManager::CSceneManager::Get().GetDrawSystem();
	//--- インスタンシング依頼
	if (!name.empty())
	{
		sys->SetInstanchingMesh(name, mesh);
	}
	else
	{
		sys->SetInstanchingMesh(
			std::string(std::to_string(mesh->GetIndexNum()) + std::to_string(mesh->GetMaterial()->GetFloat())),
			mesh
		);
	}
}


#if BUILD_MODE

void CMeshRenderer::ImGuiDebug()
{
	ImGui::Text("BSphere:%f", GetBSRadius());
	ImGui::Checkbox("Static", (bool*)&m_nStaticMode);
	
	ImGui::BeginTabBar("Material");
	if (ImGui::BeginTabItem("Diffuse"))
	{
		ImGui::ColorPicker4("Diffuse", (float*)&m_MeshMaterial.m_Diffuse);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Ambient"))
	{
		ImGui::ColorPicker4("Ambient", (float*)&m_MeshMaterial.m_Ambient);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular"))
	{
		ImGui::ColorPicker4("Specular", (float*)&m_MeshMaterial.m_Specular);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Emissive"))
	{
		ImGui::ColorPicker4("Emissive", (float*)&m_MeshMaterial.m_Emissive);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Power"))
	{
		ImGui::DragFloat("Power", (float*)&m_MeshMaterial.m_Power);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	//CRenderer::ImGuiDebug();

}

#endif // BUILD_MODE