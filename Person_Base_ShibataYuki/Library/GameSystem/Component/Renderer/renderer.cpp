//=========================================================
// [renderer.cpp] 
// �쐬:2022/06/27
// �X�V:2023/02/07 ps,vs������ǉ�
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <Application/Application.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;


//==========================================================
// �R���X�g���N�^
//==========================================================
CRenderer::CRenderer()
	:m_bVisible(true), m_nDrawIdx(-1)
{
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CRenderer::CRenderer(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bVisible(true), m_vColor(1,1,1,1)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CRenderer::~CRenderer()
{
	if(auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem(); sys)
		sys->ExecutSystem(m_nDrawIdx);
}

//==========================================================
// �`��\��
//==========================================================
void CRenderer::DrawRequest()
{
	// ���̂��o�^����Ă���̂ŗ��E
	if (m_nDrawIdx != -1)
		return;

	if (auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem(); sys)
		m_nDrawIdx = sys->RegistToSystem(BaseToDerived<CRenderer>());
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CRenderer::OnLoad()
{
	Init();
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
	static bool windowDisp = false;
	auto pSM = Application::Get()->GetSystem<MySpace::Graphics::CAssetsManager>()->GetShaderManager();
	
	// shader�I���A�\��
	if (auto psName = pSM->ImGuiGetPixelShader(m_strPixelShader); !psName.empty())
	{
		m_strPixelShader = psName;
	}

	if (auto vsName = pSM->ImGuiGetVertexShader(m_strVertexShader); !vsName.empty())
	{
		m_strVertexShader = vsName;
	}

	ImGui::Checkbox("render visible", &m_bVisible);

	ImGui::Checkbox("baseColor", &windowDisp);
	if (!windowDisp)
		return;
	if (ImGui::Begin("ColorWindow", &windowDisp))
	{
		Vector4 color = Vector4(m_vColor.a, m_vColor.g, m_vColor.b, m_vColor.a);
		ImGui::ColorPicker4("color4", (float*)&color);
		m_vColor = Color(color.x, color.y, color.z, color.w);
		ImGui::End();
	}

}

#endif // BUILD_MODE