//=========================================================
// [renderer.cpp] 
// çÏê¨: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- ÉCÉìÉNÉãÅ[Éhïî
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;

CRenderer::CRenderer(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_bVisible(true), m_vColor(1,1,1,1)
{

}
CRenderer::~CRenderer()
{
	if(SceneManager::CSceneManager::Get().GetActiveScene())
		SceneManager::CSceneManager::Get().GetActiveScene()->GetDrawManager()->ReleaseDraw(m_uIdx);
}
void CRenderer::RequestDraw()
{
	// ï`âÊìoò^
	m_uIdx = SceneManager::CSceneManager::Get().GetActiveScene()->GetDrawManager()->SetDraw(BaseToDerived<CRenderer>(),0);
}
void CRenderer::Awake()
{}
void CRenderer::Init()
{
	Transform()->Update();

	RequestDraw();
}
void CRenderer::Update()
{
}
bool CRenderer::Draw()
{
	// ï`âÊàÀóäÇèoÇ∑
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