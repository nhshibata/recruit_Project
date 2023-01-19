//========================================================
// [gameSceneManager.cpp]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

//--- �C���N���[�h��
#include <spellPlayer.h>
#include <fadeController.h>
#include <stageManager.h>
#include <gameSceneManager.h>
#include <spellDefine.h>

#include <CoreSystem/Sound/Sound.h>

using namespace Spell;

//========================================================
// �R���X�g���N�^
//========================================================
CGameSceneManager::CGameSceneManager()
	:m_pPlayer(nullptr)
{
}

//========================================================
// �������t���R���X�g���N�^
//========================================================
CGameSceneManager::CGameSceneManager(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// �f�X�g���N�^
//========================================================
CGameSceneManager::~CGameSceneManager()
{
	CSound::Stop(eBGM::BGM_GAME);
}

//========================================================
// �������Ăяo��
//========================================================
void CGameSceneManager::Awake()
{
	//--- ���Đ�
	CSound::Play(eBGM::BGM_GAME);

	//--- �t�F�[�h
	auto obj = CGameObject::FindGameObjectWithTag(Spell::TAG_FADE);
	m_pFade = obj.lock()->GetComponent<CFadeController>();
	m_pFade.lock()->SetTypeFunction<CGameSceneManager>(CFadeController::E_FadeState::FADE_IN, &CGameSceneManager::TimeFunc, this);
	m_pFade.lock()->SetTypeFunction<CGameSceneManager>(CFadeController::E_FadeState::FADE_OUT, &CGameSceneManager::TimeFunc, this);

}

//========================================================
// ������
//========================================================
void CGameSceneManager::Init()
{
	{
		auto obj = CGameObject::CreateObject().lock();
		m_pPlayer = obj->AddComponent<CSpellPlayer>().get();
	}

	{
		auto obj = CGameObject::CreateObject().lock();
		auto stage = obj->AddComponent<CStageManager>();
		stage->StartBuild(FORDER_DIR(Data / CSV / stage01.csv));
	}

}

//========================================================
// �X�V
//========================================================
void CGameSceneManager::Update()
{
	if (m_pFade.lock()->IsFadeProcess())
	{
		CFps::Get()->SetTimeScale(0);
		return;
	}

}

//========================================================
// ���Ԃ�ʏ�ɖ߂�
// �֐��|�C���^�����n���p�֐�
//========================================================
void CGameSceneManager::TimeFunc()
{
	CFps::Get()->SetTimeScale(1);
}