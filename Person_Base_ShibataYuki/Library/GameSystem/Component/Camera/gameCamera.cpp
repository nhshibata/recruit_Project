//=========================================================
// [gameCamera.h]
// �쐬:2022/07/19
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Camera/gameCamera.h>

using namespace MySpace::Game;

CGameCamera::CGameCamera()
{

}
CGameCamera::CGameCamera(std::shared_ptr<CGameObject> owner)
	:CCamera(owner)
{

}
CGameCamera::~CGameCamera()
{

}

void CGameCamera::Awake()
{

}
void CGameCamera::Init()
{

}
void CGameCamera::Update()
{

}