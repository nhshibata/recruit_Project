//==========================================================
// [gameManager.cpp]
//----------------------------------------------------------
// �쐬:2023/05/21
// 
//==========================================================

#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/gameManager.h>
#include <GameSystem/DarkGame/darkPlayer.h>
#include <GameSystem/DarkGame/enemyBase.h>
#include <GameSystem/DarkGame/gameGoal.h>
#include <GameSystem/DarkGame/gameStartPosition.h>
#include <GameSystem/DarkGame/pauseMenu.h>
#include <Application/Application.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameManager::CGameManager()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameManager::CGameManager(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// ����������
//==========================================================
void CGameManager::Awake()
{
	// �Q�Ƃ̒T��
	{
		auto obj = CGameObject::FindGameObjectWithTag(CGameGoal::TAG);
		m_pGoal = obj.lock()->GetComponent<CGameGoal>();
	}
	{
		auto obj = CGameObject::FindGameObjectWithTag(CGameStartPosition::TAG);
		m_pStartPos = obj.lock()->GetComponent<CGameStartPosition>();
	}
	{
		auto obj = CGameObject::FindGameObjectWithTag(CDarkPlayer::TAG);
		m_pPlayer = obj.lock()->GetComponent<CDarkPlayer>();
	}
	{
		auto obj = CGameObject::FindGameObjectWithTag(CImageSwitch::TAG);
		m_pUI = obj.lock()->GetComponent<CImageSwitch>();
	}
	{
		auto obj = CGameObject::FindGameObjectWithTag(CPauseMenu::TAG);
		m_pPause = obj.lock()->GetComponent<CPauseMenu>();
	}
}

//==========================================================
// ����������
//==========================================================
void CGameManager::Init()
{
	StopMode(true);
	SetState(EState::Start);
}

//==========================================================
// �X�V
//==========================================================
void CGameManager::Update()
{
	// ���ݏ󋵂ŏ����ؑ�
	switch (m_eCurrentState)
	{
		case DarkGame::CGameManager::EState::Start:
			// �X�^�[�g�܂ŊԂ���������
			if (!m_pUI.lock()->IsAnim())
			{
				SetState(EState::Play);
			}
			break;
		case DarkGame::CGameManager::EState::Play:
		{
			// �����ꗎ�������珉���ʒu��
			auto pos = m_pPlayer.lock()->Transform()->GetPos();
			if (pos.y < -100.0f)
			{
				m_pPlayer.lock()->Transform()->SetPos(m_pStartPos.lock()->Transform()->GetPos());
			}

			// �S�[���m�F
			if (m_pGoal.lock()->IsGoal())
			{
				SetState(EState::Goal);
			}

			// �Q�[���I�[�o�[�����B����
			//if(m_pPlayer)
			break;
		}
		case DarkGame::CGameManager::EState::Over:
			// UI�X�V?
			if (!m_pUI.lock()->IsAnim())
			{
				// �V�[���ؑ�

			}
			break;
		case DarkGame::CGameManager::EState::Goal:
			// �S�[�����o�I�����ɃV�[���ؑ�
			if (!m_pUI.lock()->IsAnim())
			{
				// �V�[���ؑ�

			}
			break;
		case DarkGame::CGameManager::EState::Pause:
		{
			// ��~��ʍX�V
			// �|�[�Y���̏����m�F
			CPauseMenu::ESelect select = (CPauseMenu::ESelect)m_pPause.lock()->Select();
			switch (select)
			{
				case DarkGame::CPauseMenu::ESelect::Title:
					SetState(EState::Play);
					break;
				case DarkGame::CPauseMenu::ESelect::Quit:
					SetState(EState::Quit);
					break;
				case DarkGame::CPauseMenu::ESelect::Max:
					break;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}


//==========================================================
// ��ԕύX����
//==========================================================
void CGameManager::SetState(const EState newState)
{
	// old
	if (m_eCurrentState == EState::Pause)
	{
		m_pPause.lock()->UIEnable(false);
	}

	m_eCurrentState = newState;

	switch (m_eCurrentState)
	{
		case DarkGame::CGameManager::EState::Start:
			// ��ʕ\��
			StopMode(false);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Start);
			break;
		case DarkGame::CGameManager::EState::Play:
			// ����ύX�����Ȃ�߂�
			StopMode(false);
			break;
		case DarkGame::CGameManager::EState::Over:
			// UI���o�\��
			StopMode(true);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Over);
			break;
		case DarkGame::CGameManager::EState::Goal:
			// UI���o�\��
			StopMode(true);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Goal);
			break;
		case DarkGame::CGameManager::EState::Pause:
			// ��~��ʕ\��
			StopMode(true);
			m_pPause.lock()->UIEnable(true);
			break;
		case DarkGame::CGameManager::EState::Quit:
			// �I������
			Application::Get()->Quit();
			break;
		default:
			break;
	}
}

//==========================================================
// ��~���
// �������ɂ��ω�
//==========================================================
void CGameManager::StopMode(const bool& stop)
{
	m_pPlayer.lock()->SetMoving(stop);

	// �G
	auto enemys = CGameObject::FindGameObjectsWithTag("Enemy");
	for (const auto & obj : enemys)
	{
		auto enemy = obj.lock()->GetComponent<CEnemyBase>();
		enemy.lock()->SetMoving(stop);
	}
}