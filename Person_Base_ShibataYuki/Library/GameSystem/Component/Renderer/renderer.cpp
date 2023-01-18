//=========================================================
// [renderer.cpp] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CRenderer::CRenderer(std::shared_ptr<CGameObject> owner)
	:CComponent(owner),m_bVisible(true), m_vColor(1,1,1,1)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CRenderer::~CRenderer()
{
	if(auto sys = SceneManager::CSceneManager::Get().GetDrawSystem(); sys)
#pragma warning(suppress: 26444)
		sys->ExecutSystem(m_nDrawIdx);
}

//==========================================================
// �`��\��
//==========================================================
void CRenderer::DrawRequest()
{
	if (auto sys = SceneManager::CSceneManager::Get().GetDrawSystem(); sys)
		m_nDrawIdx = sys->RegistToSystem(BaseToDerived<CRenderer>());
}

//==========================================================
// �������Ăяo��
//==========================================================
void CRenderer::Awake()
{
}

//==========================================================
// ������
//==========================================================
void CRenderer::Init()
{
	if(Transform())
		Transform()->Update();

	DrawRequest();
}

//==========================================================
// �X�V
//==========================================================
void CRenderer::Update()
{
}

//==========================================================
// �`��
//==========================================================
bool CRenderer::Draw()
{
	// �`��m�F
	return m_bVisible && GetOwner()->IsVision();
}

//==========================================================
// ���C���[�ݒ�
//==========================================================
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
		Vector4 color = Vector4(m_vColor.a, m_vColor.g, m_vColor.b, m_vColor.a);
		ImGui::ColorPicker4("color4", (float*)&color);
		//ImGui::ColorEdit4("color", (float*)&color);
		m_vColor = Color(color.x, color.y, color.z, color.w);
	ImGui::End();
	}

}

#endif // BUILD_MODE