//=========================================================
// [renderer.cpp] 
// çÏê¨: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- ÉCÉìÉNÉãÅ[Éhïî
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

CMeshRenderer::CMeshRenderer() 
	:m_vCenter(0, 0, 0), m_bLightEnable(true),m_nStaticMode(static_cast<int>(EStaticMode::NONE))
{
}
CMeshRenderer::CMeshRenderer(std::shared_ptr<CGameObject> owner)
	: CRenderer(owner), m_vCenter(0,0,0), m_bLightEnable(true), m_nStaticMode(static_cast<int>(EStaticMode::NONE))
{
	m_MeshMaterial = CMeshMaterial(
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 1.0f, 0.0f, 0.3f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f);
}
CMeshRenderer::~CMeshRenderer()
{

}
void CMeshRenderer::Awake()
{

}
void CMeshRenderer::Init()
{
	// ï`âÊìoò^
	CRenderer::Init();
}
void CMeshRenderer::Update()
{

}
bool CMeshRenderer::Draw()
{
	return CRenderer::Draw();
}
Vector3 CMeshRenderer::GetCenter(int n) 
{
	return m_vCenter + Transform()->GetPos(); 
}

#if BUILD_MODE

void CMeshRenderer::ImGuiDebug()
{
	ImGui::Text("Material");
	ImGui::Checkbox("Static", (bool*)&m_nStaticMode);
	ImGui::ColorPicker4("Diffuse", (float*)&m_MeshMaterial.m_Diffuse);
	ImGui::ColorPicker4("Ambient", (float*)&m_MeshMaterial.m_Ambient);
	ImGui::ColorPicker4("Specular", (float*)&m_MeshMaterial.m_Specular);
	ImGui::ColorPicker4("Emissive", (float*)&m_MeshMaterial.m_Emissive);
	ImGui::DragFloat("Material", (float*)&m_MeshMaterial.m_Power);
	//CRenderer::ImGuiDebug();
}

#endif // BUILD_MODE