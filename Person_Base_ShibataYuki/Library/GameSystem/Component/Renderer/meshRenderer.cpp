//=========================================================
// [renderer.cpp] 
// ì¬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- ƒCƒ“ƒNƒ‹[ƒh•”
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

CMeshRenderer::CMeshRenderer() 
	:m_vCenter(0, 0, 0), m_bLightEnable(true)
{
}
CMeshRenderer::CMeshRenderer(std::shared_ptr<CGameObject> owner)
	: CRenderer(owner), m_vCenter(0,0,0)
{
	
}
CMeshRenderer::~CMeshRenderer()
{

}
void CMeshRenderer::Awake()
{

}
void CMeshRenderer::Init()
{
	// •`‰æ“o˜^
	CRenderer::Init();
	//SceneManager::CSceneManager::Get().GetActiveScene()->GetDrawManager()->SetObjectDraw(GetPtr());
	//SceneManager::CSceneManager::Get().GetActiveScene()->GetDrawManager()->SetDraw(BaseToDerived<CRenderer>());
}
void CMeshRenderer::Update()
{

}
bool CMeshRenderer::Draw()
{
	return CRenderer::Draw();
}
Vector3 CMeshRenderer::GetCenter(int n) 
{
	return m_vCenter + Transform()->GetPos(); 
}
