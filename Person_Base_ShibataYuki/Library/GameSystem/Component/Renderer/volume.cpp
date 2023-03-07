//==========================================================
// [volume.cpp]
//----------------------------------------------------------
// 
//==========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/volume.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Manager/volumeManager.h>

#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/negative.h>
#include <GraphicsSystem/PostProcess/monochrome.h>
#include <GraphicsSystem/PostProcess/outline.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CVolume::CVolume()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CVolume::CVolume(CGameObject::Ptr owner)
	:CComponent(owner), m_nPriority(0), m_nID(-1)
{

}

//==========================================================
// デストラクタ
//==========================================================
CVolume::~CVolume()
{
	if (auto mgr = CSceneManager::Get()->GetDrawSystem()->GetVolumeManager(); mgr)
	{
		mgr->ExecutSystem(m_nID);
	}

}

//==========================================================
// 
//==========================================================
void CVolume::Awake()
{

}

//==========================================================
// 初期化
//==========================================================
void CVolume::Init()
{
	if (auto mgr = CSceneManager::Get()->GetDrawSystem()->GetVolumeManager(); mgr)
	{
		m_nID = mgr->RegistToSystem(this);
	}

}

//==========================================================
// レイヤー確認
//==========================================================
bool CVolume::IsLayer(const int layerNo)
{
	if (GetOwner()->GetLayerPtr()->GetLayer() & layerNo)
	{
		return true;
	}

	return false;
}


#if BUILD_MODE

void CVolume::ImGuiDebug()
{
	static std::vector<std::string> effList = {
		"Bloom",
		"Monochrome",
		"Negative",
		"Outline",
	};

	ImGui::DragInt(u8"volume 優先度", &m_nPriority, 1, 0, 10);

	int n = Debug::DispComboSelect(effList, "Select", -1);
	if (n != -1)
		m_pPost.reset();

	switch (n)
	{
		case 0:
			m_pPost = std::make_unique<CBloom>();
			break;
		case 1:
			m_pPost = std::make_unique<CMonochrome>();
			break;
		case 2:
			m_pPost = std::make_unique<CNegative>();
			break;
		case 3:
			m_pPost = std::make_unique<COutline>();
			break;
		default:
			break;
	}
	
	if (m_pPost)
	{
		// パラメータ表示
		m_pPost->ImGuiDebug();
		// 画面表示
		ImGui::Image(m_pPost->GetResource(), ImVec2(100,100));
	}

}

#endif // BUILD_MODE