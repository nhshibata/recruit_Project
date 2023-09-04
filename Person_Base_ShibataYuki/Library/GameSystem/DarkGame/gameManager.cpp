//==========================================================
// [gameManager.cpp]
//----------------------------------------------------------
// 作成:2023/05/21
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
// コンストラクタ
//==========================================================
CGameManager::CGameManager()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CGameManager::CGameManager(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// 生成時処理
//==========================================================
void CGameManager::Awake()
{
	// 参照の探索
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
// 初期化処理
//==========================================================
void CGameManager::Init()
{
	StopMode(true);
	SetState(EState::Start);
}

//==========================================================
// 更新
//==========================================================
void CGameManager::Update()
{
	// 現在状況で処理切替
	switch (m_eCurrentState)
	{
		case DarkGame::CGameManager::EState::Start:
			// スタートまで間を持たせる
			if (!m_pUI.lock()->IsAnim())
			{
				SetState(EState::Play);
			}
			break;
		case DarkGame::CGameManager::EState::Play:
		{
			// 万が一落下したら初期位置に
			auto pos = m_pPlayer.lock()->Transform()->GetPos();
			if (pos.y < -100.0f)
			{
				m_pPlayer.lock()->Transform()->SetPos(m_pStartPos.lock()->Transform()->GetPos());
			}

			// ゴール確認
			if (m_pGoal.lock()->IsGoal())
			{
				SetState(EState::Goal);
			}

			// ゲームオーバー条件達成時
			//if(m_pPlayer)
			break;
		}
		case DarkGame::CGameManager::EState::Over:
			// UI更新?
			if (!m_pUI.lock()->IsAnim())
			{
				// シーン切替

			}
			break;
		case DarkGame::CGameManager::EState::Goal:
			// ゴール演出終了時にシーン切替
			if (!m_pUI.lock()->IsAnim())
			{
				// シーン切替

			}
			break;
		case DarkGame::CGameManager::EState::Pause:
		{
			// 停止画面更新
			// ポーズ側の処理確認
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
// 状態変更処理
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
			// 画面表示
			StopMode(false);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Start);
			break;
		case DarkGame::CGameManager::EState::Play:
			// 音を変更したなら戻す
			StopMode(false);
			break;
		case DarkGame::CGameManager::EState::Over:
			// UI演出表示
			StopMode(true);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Over);
			break;
		case DarkGame::CGameManager::EState::Goal:
			// UI演出表示
			StopMode(true);
			m_pUI.lock()->StartAnim(CImageSwitch::EDispImage::Goal);
			break;
		case DarkGame::CGameManager::EState::Pause:
			// 停止画面表示
			StopMode(true);
			m_pPause.lock()->UIEnable(true);
			break;
		case DarkGame::CGameManager::EState::Quit:
			// 終了処理
			Application::Get()->Quit();
			break;
		default:
			break;
	}
}

//==========================================================
// 停止状態
// 引き数により変化
//==========================================================
void CGameManager::StopMode(const bool& stop)
{
	m_pPlayer.lock()->SetMoving(stop);

	// 敵
	auto enemys = CGameObject::FindGameObjectsWithTag("Enemy");
	for (const auto & obj : enemys)
	{
		auto enemy = obj.lock()->GetComponent<CEnemyBase>();
		enemy.lock()->SetMoving(stop);
	}
}