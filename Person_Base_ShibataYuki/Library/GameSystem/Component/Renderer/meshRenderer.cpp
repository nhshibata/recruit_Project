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
{

}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CMeshRenderer::CMeshRenderer(std::shared_ptr<CGameObject> owner)
	: CRenderer(owner), m_vCenter(0, 0, 0), m_bLightEnable(true), m_fBSRadius(1)
	, m_nStaticMode(static_cast<int>(EStaticMode::NONE)), m_bShadow(false)
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),	// wはﾃｸｽﾁｬ有無に使われている
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),	// wはpowerに使われている
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
	// デフォルトシェーダー
	m_strPixelShader = "PS_Mesh";
	m_strVertexShader = "VS_Mesh";
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
void CMeshRenderer::SetInstancing(CMesh* mesh, std::string name, DirectX::XMUINT4 vFlag)
{
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();

	// wはpowerに使われている
	Vector4 spec(m_MeshMaterial.m_Specular.x, m_MeshMaterial.m_Specular.y, m_MeshMaterial.m_Specular.z, m_MeshMaterial.m_Power);
	
	RENDER_DATA rd = RENDER_DATA(Transform()->GetWorldMatrix(),
								 m_MeshMaterial.m_Ambient, m_MeshMaterial.m_Diffuse,
								 spec, m_MeshMaterial.m_Emissive,
								 vFlag);

	//--- インスタンシング依頼
	if (!name.empty())
	{
		sys->SetInstanchingMesh(
			name,
			m_strPixelShader,
			m_strVertexShader,
			rd,
			mesh);
	}
	else
	{
		sys->SetInstanchingMesh(
			std::string(std::to_string(mesh->GetIndexNum()) + std::to_string(mesh->GetMaterial()->GetFloat())),
			m_strPixelShader,
			m_strVertexShader,
			rd,
			mesh);
	}

}


#if BUILD_MODE

void CMeshRenderer::ImGuiDebug()
{
	CRenderer::ImGuiDebug();

	ImGui::Text("BSphere:%f", GetBSRadius());
	ImGui::Checkbox("Static", (bool*)&m_nStaticMode);
	ImGui::SameLine();
	ImGui::Checkbox("Shadow", (bool*)&m_bShadow);

	ImGui::BeginTabBar("Material");
	if (ImGui::BeginTabItem("Diffuse"))
	{
		ImGui::ColorPicker4("DiffuseColor", (float*)&m_MeshMaterial.m_Diffuse);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Ambient"))
	{
		ImGui::ColorPicker4("AmbientColor", (float*)&m_MeshMaterial.m_Ambient);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular"))
	{
		ImGui::ColorPicker4("SpecularColor", (float*)&m_MeshMaterial.m_Specular);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Emissive"))
	{
		ImGui::ColorPicker4("EmissiveColor", (float*)&m_MeshMaterial.m_Emissive);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Power"))
	{
		ImGui::DragFloat("Power", (float*)&m_MeshMaterial.m_Power);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	
}

#endif // BUILD_MODE