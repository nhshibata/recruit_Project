
//--- �C���N���[�h��
#include <GameSystem/components.h>

#include <DebugSystem/typeSaveManager.h>

using namespace MySpace::Debug;
using namespace MySpace::Game;

//==========================================================
// �^�̕ۑ�
// ��x�����������s����Ȃ�
//==========================================================
void CreateComponentType()
{
	static bool ones = true;
	if (!ones)return;
	ones = false;

	// instance�̊l��
	CTypeSaveManager* mgr = CTypeSaveManager::Get();

	// ����	
	mgr->SetComponentSave<CTransform>();
	mgr->SetComponentSave<CRectTransform>();
	mgr->SetComponentSave<CRigidbody>();

	mgr->SetComponentSave<CCamera>();
	mgr->SetComponentSave<CStackCamera>();
	mgr->SetComponentSave<CLayerCamera>();
	mgr->SetComponentSave<CGameCamera>();
	mgr->SetComponentSave<CDebugCamera>();

	mgr->SetComponentSave<CDirectionalLight>();
	mgr->SetComponentSave<CSphereCollision>();
	mgr->SetComponentSave<CBoxCollision>();

	mgr->SetComponentSave<CMeshRenderer>();
	mgr->SetComponentSave<CBillboardRenderer>();
	mgr->SetComponentSave<CPolygonRenderer>();
	mgr->SetComponentSave<CSphereRenderer>();
	mgr->SetComponentSave<CBoxRenderer>();
	mgr->SetComponentSave<CModelRenderer>();
	mgr->SetComponentSave<CTextRenderer>();
	mgr->SetComponentSave<CEffekseerRenderer>();
	mgr->SetComponentSave<CVolume>();

	mgr->SetComponentSave<AI::CAISystem>();
	mgr->SetComponentSave<AI::CAIAgent>();

	mgr->SetComponentSave<Spell::CSpellPlayer>();

	mgr->SetComponentSave<CActionPlayer>();

	mgr->SetComponentSave<CRenderer>();
	mgr->SetComponentSave<CLight>();
	mgr->SetComponentSave<CCollision>();
}