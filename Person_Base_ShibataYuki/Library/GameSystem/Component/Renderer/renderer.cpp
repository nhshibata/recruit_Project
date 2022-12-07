//=========================================================
// [renderer.cpp] 
// çÏê¨: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- ÉCÉìÉNÉãÅ[Éhïî
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
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
	if(auto sys = SceneManager::CSceneManager::Get().GetDrawSystem(); sys)
		sys->ExecutSystem(m_nDrawIdx);
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

void CRenderer::SetLayer(int value) 
{
	GetOwner()->SetLayer(value); 
}

#ifdef BUILD_MODE

void CRenderer::ImGuiDebug()
{
	static bool disp = false;
	ImGui::Text(u8"Renderer");
	ImGui::Checkbox("bool", &m_bVisible);
	if (ImGui::Begin(u8"ColorWindow", &disp))
	{
		if (!disp)
			return;
		Vector4 color = Vector4(m_vColor.a, m_vColor.g, m_vColor.b, m_vColor.a);
		ImGui::ColorPicker4("color4", (float*)&color);
		//ImGui::ColorEdit4("color", (float*)&color);
		m_vColor = Color(color.x, color.y, color.z, color.w);
	}
	ImGui::End();
}
#endif // BUILD_MODE