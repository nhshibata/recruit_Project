//=========================================================
// [renderer.cpp] 
// çÏê¨: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- ÉCÉìÉNÉãÅ[Éhïî
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

CRenderer::CRenderer(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_bVisible(true), m_vColor(1,1,1,1)
{

}
CRenderer::~CRenderer()
{
	SceneManager::CSceneManager::Get().GetDrawSystem()->ExecutSystem(m_nDrawIdx);
}
void CRenderer::RequestDraw()
{
	// ï`âÊìoò^
	m_nDrawIdx = SceneManager::CSceneManager::Get().GetDrawSystem()->RegistToSystem(BaseToDerived<CRenderer>());
}
void CRenderer::Awake()
{}
void CRenderer::Init()
{
	if(Transform())
		Transform()->Update();

	RequestDraw();
}
void CRenderer::Update()
{
}
bool CRenderer::Draw()
{
	// ï`âÊämîF
	return m_bVisible && GetOwner()->IsVision();
}

#ifdef BUILD_MODE

void CRenderer::ImGuiDebug()
{
	ImGui::Text(u8"Renderer");
	ImGui::Checkbox("bool", &m_bVisible);
	ImGui::ColorPicker4("color", (float*)&m_vColor);

}
#endif // BUILD_MODE