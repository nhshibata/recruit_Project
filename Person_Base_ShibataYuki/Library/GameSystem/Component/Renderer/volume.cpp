//==========================================================
// [volume.cpp]
//----------------------------------------------------------
// 
//==========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GameSystem/Component/Renderer/volume.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Manager/volumeManager.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CVolume::CVolume()
	:m_nID(-1)
{
	if (auto mgr = CSceneManager::Get()->GetDrawSystem()->GetVolumeManager(); mgr)
	{
		m_nID = mgr->RegistToSystem(this);
	}
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CVolume::CVolume(CGameObject::Ptr owner)
	:CComponent(owner), m_nPriority(0), m_nID(-1)
{

}

//==========================================================
// �f�X�g���N�^
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
// ������
//==========================================================
void CVolume::Init()
{
	// �ǂݍ��ݎ��ɂ�-1�ȊO�ɂȂ��Ă���
	if (m_nID != -1)
		return;

	if (auto mgr = CSceneManager::Get()->GetDrawSystem()->GetVolumeManager(); mgr)
	{
		m_nID = mgr->RegistToSystem(this);
	}

}

//==========================================================
// ���C���[�m�F
//==========================================================
bool CVolume::IsLayer(const int layerBit)
{
	if (GetOwner()->GetLayerPtr()->GetLayer() & layerBit)
	{
		return true;
	}

	return false;
}

//==========================================================
// �ǉ�
//==========================================================
void CVolume::AddRendererID(const int nID)
{
	const int layer = GetLayer();
	if (!m_aRenderIDCash.count(layer))
	{
		m_aRenderIDCash[layer] = std::vector<int>();
	}

	m_aRenderIDCash[layer].push_back(nID);
}

//==========================================================
// �N���A
//==========================================================
void CVolume::ResetRenderCash()
{
	m_aRenderIDCash.clear();
}

//==========================================================
// �擾
//==========================================================
std::vector<int> CVolume::GetRenderCash()const
{
	if (m_aRenderIDCash.size() == 0)
		return std::vector<int>();
	return m_aRenderIDCash.begin()->second;
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

	Debug::SetTextAndAligned(u8"volume �D��x");
	ImGui::DragInt("##volume �D��x", &m_nPriority, 1, 0, 10);

	Debug::SetTextAndAligned(u8"Select Effect");
	int select = Debug::DispComboSelect(effList, "##Select", -1);
	if (select != -1)
		m_pPost.reset();
	switch (select)
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
		// �p�����[�^�\��
		m_pPost->ImGuiDebug();

		// ��ʕ\��
		ImGui::Image(m_pPost->GetResource(), ImVec2(CScreen::GetWidth()/10, CScreen::GetHeight()/10));
	}

}

#endif // BUILD_MODE