//=========================================================
// [renderer.cpp] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Renderer/effekseerRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/GameObject/gameObject.h>

#include <GraphicsSystem/DirectX/DXDevice.h>

#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/imguiManager.h>
#include <CoreSystem/File/filePath.h>

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

#define EFFECT_PATH1		FORDER_DIR(Data/effect/)

// コンストラクタ
CEffekseerRenderer::CEffekseerRenderer(std::shared_ptr<CGameObject> owner)
	:CRenderer(owner),m_bLoop(false),m_fAngle(0.0f),m_nHandle(-1),m_pEffekseer(nullptr)
{
	
}
CEffekseerRenderer::~CEffekseerRenderer()
{
	
}
void CEffekseerRenderer::Awake()
{
	m_pEffekseer = &CEffekseer::Get();
}
void CEffekseerRenderer::Init()
{
	// 描画依頼
	CRenderer::Init();
}
void CEffekseerRenderer::Update()
{
#if BUILD_MODE
	if (!m_bRead || Debug::ImGuiManager::Get().CheckPlayMode())
		return;
#endif // 0

	m_nHandle = m_pEffekseer->Move(m_nHandle, Transform()->GetPos());

	// 終了確認
	if (!m_pEffekseer->Exists(m_nHandle))
	{
		CGameObject::Destroy(GetOwner(0));
	}
}
bool CEffekseerRenderer::Draw()
{
	if (!CRenderer::Draw())
	{
		m_pEffekseer->Stop(m_nHandle);

		return false;
	}
	if (m_nHandle == -1 && m_bLoop)
	{
		XMFLOAT3 rot = Transform()->GetRot();
		m_pEffekseer->Load(m_EffectName);
		m_nHandle = m_pEffekseer->Play(m_EffectName, Transform()->GetPos(), Transform()->GetScale(), XMFLOAT4(rot.x, rot.y, rot.z, m_fAngle));
	}

	return true;
}
void CEffekseerRenderer::SetImageName(std::u16string name)
{
	m_EffectName = name;
	XMFLOAT3 rot = Transform()->GetRot();
	m_pEffekseer->Load(name);
	m_nHandle = m_pEffekseer->Play(name, Transform()->GetPos(), Transform()->GetScale(), XMFLOAT4(rot.x, rot.y, rot.z, m_fAngle));
}

#ifdef BUILD_MODE

void CEffekseerRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_FileNameList;
	static std::vector<std::u16string> s_FileList;

	if (s_FileNameList.empty() || ImGui::Button(u8"エフェクト reload"))
	{
		MySpace::System::CFilePath file;
		s_FileNameList = file.GetAllFileName(EFFECT_PATH1, ".efkefc");
		s_FileList = file.GetAllFileNameTo16(EFFECT_PATH1, ".efkefc");
	}

	// FIXME: 確認必須 empty
	if (std::u16string name = DispFileMenuBar16(s_FileNameList, s_FileList, "effect"); !name.empty())
	{
		m_EffectName = name;
		m_bRead = true;
	}

	ImGui::Text(u8"name%zu", m_EffectName.c_str());
	ImGui::InputFloat(u8"angle", &m_fAngle);
	if (ImGui::Button(u8"loop"))
	{
		m_bLoop ^= true;
	}
}
#endif // BUILD_MODE