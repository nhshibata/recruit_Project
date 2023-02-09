//=========================================================
// [renderer.cpp] 
// 作成:2022/06/27
// 更新:2023/02/07 ps,vs文字列追加
//---------------------------------------------------------
//=========================================================

//--- インクルード部
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
// コンストラクタ
//==========================================================
CRenderer::CRenderer(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bVisible(true), m_vColor(1,1,1,1)
{

}

//==========================================================
// デストラクタ
//==========================================================
CRenderer::~CRenderer()
{
	if(auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem(); sys)
		sys->ExecutSystem(m_nDrawIdx);
}

//==========================================================
// 描画申請
//==========================================================
void CRenderer::DrawRequest()
{
	// 何故か登録されているので離脱
	if (m_nDrawIdx != -1)
		return;

	if (auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem(); sys)
		m_nDrawIdx = sys->RegistToSystem(BaseToDerived<CRenderer>());
}

//==========================================================
// 読み込み時呼び出し
//==========================================================
void CRenderer::OnLoad()
{
	Init();
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CRenderer::Awake()
{
}

//==========================================================
// 初期化
//==========================================================
void CRenderer::Init()
{
	if(Transform())
		Transform()->Update();

	DrawRequest();
}

//==========================================================
// 更新
//==========================================================
void CRenderer::Update()
{
}

//==========================================================
// 描画
//==========================================================
bool CRenderer::Draw()
{
	// 描画確認
	return m_bVisible && GetOwner()->IsVision();
}

//==========================================================
// レイヤー設定
//==========================================================
void CRenderer::SetLayer(int value) 
{
	GetOwner()->SetLayer(value); 
}


#ifdef BUILD_MODE

void CRenderer::ImGuiDebug()
{
	static bool disp = false;
	auto pSM = Application::Get()->GetSystem<MySpace::Graphics::CAssetsManager>()->GetShaderManager();
	
	// shader選択、表示
	if (auto psName = pSM->ImGuiGetPixelShader(m_strPixelShader); !psName.empty())
	{
		m_strPixelShader = psName;
	}

	if (auto vsName = pSM->ImGuiGetVertexShader(m_strVertexShader); !vsName.empty())
	{
		m_strVertexShader = vsName;
	}

	ImGui::Checkbox("render visible", &m_bVisible);

	ImGui::Checkbox("baseColor", &disp);
	if (!disp)
		return;
	if (ImGui::Begin("ColorWindow", &disp))
	{
		Vector4 color = Vector4(m_vColor.a, m_vColor.g, m_vColor.b, m_vColor.a);
		ImGui::ColorPicker4("color4", (float*)&color);
		m_vColor = Color(color.x, color.y, color.z, color.w);
		ImGui::End();
	}

}

#endif // BUILD_MODE