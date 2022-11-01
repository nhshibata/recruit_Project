//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 派生クラス
//=========================================================

//--- インクルード部
#include <GameSystem/Scene/gameScene.h>
//#include <GameSystem/Component/Collision/collision.h>

using namespace MySpace::SceneManager;

//コンストラクタ
CGameScene::CGameScene()
	:CScene("Game")
{

}
// デストラクタ
CGameScene::~CGameScene()
{

}
void CGameScene::Init()
{
	CScene::Init();
}
void CGameScene::Uninit()
{
	CScene::Uninit();
}
void CGameScene::Update()
{
	CScene::Update();
}
void CGameScene::Draw()
{
	CScene::Draw();
}
