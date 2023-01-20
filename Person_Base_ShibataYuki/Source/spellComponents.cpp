


//--- インクルード部
#include <spellComponents.h>

#include <DebugSystem/typeSaveManager.h>


//CEREAL_REGISTER_TYPE(CPlayer)

//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, CPlayer)

using namespace MySpace::Debug;
using namespace MySpace::Game;


//==========================================================
// 型の保存
// 一度だけしか実行されない
//==========================================================
void CreateComponentSpell()
{
	static bool ones = true;
	if (!ones)return;
	ones = false;

	// instanceの獲得
	CTypeSaveManager* mgr = CTypeSaveManager::Get();

	// 生成	
	mgr->SetComponentSave<Spell::CGameManager>();
	mgr->SetComponentSave<Spell::CGameSceneManager>();
	mgr->SetComponentSave<Spell::CStageManager>();
	mgr->SetComponentSave<Spell::CSpellPlayer>();
	mgr->SetComponentSave<Spell::CTitleMove>();
	mgr->SetComponentSave<Spell::CTitleSceneManager>();
	mgr->SetComponentSave<Spell::CGameCamera>();

}