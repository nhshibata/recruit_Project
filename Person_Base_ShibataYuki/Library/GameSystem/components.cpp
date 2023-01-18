
//--- インクルード部
#include <GameSystem/components.h>

#include <DebugSystem/typeSaveManager.h>


//CEREAL_REGISTER_TYPE(CPlayer)

//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, CPlayer)

using namespace MySpace::Debug;
using namespace MySpace::Game;


//==========================================================
// 型の保存
// 一度だけしか実行されない
//==========================================================
void CreateComponentType()
{
	static bool ones = true;
	if (!ones)return;
	ones = false;

	// instanceの獲得
	CTypeSaveManager* mgr = CTypeSaveManager::Get();

	// 生成	
	mgr->SetComponentSave<CTransform>();
	mgr->SetComponentSave<CRectTransform>();
	mgr->SetComponentSave<CRigidbody>();

	mgr->SetComponentSave<CCamera>();
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

	mgr->SetComponentSave<AI::CAISystem>();
	mgr->SetComponentSave<AI::CAIAgent>();

	mgr->SetComponentSave<CRenderer>();
	mgr->SetComponentSave<CLight>();
	mgr->SetComponentSave<CCollision>();
}