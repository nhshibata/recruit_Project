#include <GameSystem/components.h>
#include <DebugSystem/typeSaveManager.h>


//CEREAL_REGISTER_TYPE(CPlayer)

//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, CPlayer)

using namespace MySpace::Debug;
using namespace MySpace::Game;

void CreateComponentType()
{
	static bool ones = true;
	if (!ones)return;
	ones = false;

	// instanceÇÃälìæ
	CTypeSaveManager& mgr = CTypeSaveManager::Get();

	// ê∂ê¨	
	mgr.SetComponentSave<CTransform>();
	mgr.SetComponentSave<CRectTransform>();
	mgr.SetComponentSave<CRigidbody>();

	mgr.SetComponentSave<CCamera>();
	mgr.SetComponentSave<CGameCamera>();
	mgr.SetComponentSave<CDebugCamera>();

	mgr.SetComponentSave<CLight>();
	mgr.SetComponentSave<CDirectionalLight>();
	mgr.SetComponentSave<CCollision>();
	mgr.SetComponentSave<CSphereCollision>();
	mgr.SetComponentSave<CBoxCollision>();

	mgr.SetComponentSave<CRenderer>();
	mgr.SetComponentSave<CMeshRenderer>();
	mgr.SetComponentSave<CPolygonRenderer>();
	mgr.SetComponentSave<CSphereRenderer>();
	mgr.SetComponentSave<CBoxRenderer>();
	mgr.SetComponentSave<CModelRenderer>();
	mgr.SetComponentSave<CTextRenderer>();
	mgr.SetComponentSave<CEffekseerRenderer>();
}