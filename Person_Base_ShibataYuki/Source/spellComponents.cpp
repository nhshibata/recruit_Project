


//--- �C���N���[�h��
#include <spellComponents.h>

#include <DebugSystem/typeSaveManager.h>


//CEREAL_REGISTER_TYPE(CPlayer)

//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, CPlayer)

using namespace MySpace::Debug;
using namespace MySpace::Game;


//==========================================================
// �^�̕ۑ�
// ��x�����������s����Ȃ�
//==========================================================
void CreateComponentSpell()
{
	static bool ones = true;
	if (!ones)return;
	ones = false;

	// instance�̊l��
	CTypeSaveManager* mgr = CTypeSaveManager::Get();

	// ����	
	mgr->SetComponentSave<Spell::CGameManager>();
	mgr->SetComponentSave<Spell::CGameSceneManager>();
	mgr->SetComponentSave<Spell::CStageManager>();
	mgr->SetComponentSave<Spell::CSpellPlayer>();
	mgr->SetComponentSave<Spell::CTitleMove>();
	mgr->SetComponentSave<Spell::CTitleSceneManager>();
	mgr->SetComponentSave<Spell::CGameCamera>();

}