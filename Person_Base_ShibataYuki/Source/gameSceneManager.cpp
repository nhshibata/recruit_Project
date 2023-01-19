//========================================================
// [gameSceneManager.cpp]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

//--- インクルード部
#include <spellPlayer.h>
#include <fadeController.h>
#include <stageManager.h>
#include <gameSceneManager.h>
#include <spellDefine.h>

#include <CoreSystem/Sound/Sound.h>

using namespace Spell;

//========================================================
// コンストラクタ
//========================================================
CGameSceneManager::CGameSceneManager()
	:m_pPlayer(nullptr)
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CGameSceneManager::CGameSceneManager(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// デストラクタ
//========================================================
CGameSceneManager::~CGameSceneManager()
{
	CSound::Stop(eBGM::BGM_GAME);
}

//========================================================
// 生成時呼び出し
//========================================================
void CGameSceneManager::Awake()
{
	//--- 音再生
	CSound::Play(eBGM::BGM_GAME);

	//--- フェード
	auto obj = CGameObject::FindGameObjectWithTag(Spell::TAG_FADE);
	m_pFade = obj.lock()->GetComponent<CFadeController>();
	m_pFade.lock()->SetTypeFunction<CGameSceneManager>(CFadeController::E_FadeState::FADE_IN, &CGameSceneManager::TimeFunc, this);
	m_pFade.lock()->SetTypeFunction<CGameSceneManager>(CFadeController::E_FadeState::FADE_OUT, &CGameSceneManager::TimeFunc, this);

}

//========================================================
// 初期化
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
// 更新
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
// 時間を通常に戻す
// 関数ポインタ引き渡し用関数
//========================================================
void CGameSceneManager::TimeFunc()
{
	CFps::Get()->SetTimeScale(1);
}