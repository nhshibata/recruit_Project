//=========================================================
//作成:2022/04/20 (水曜日)
// シーンｸﾗｽ : 派生クラス
//=========================================================

//--- インクルード部
#include <GameSystem/Scene/titleScene.h>

using namespace MySpace::SceneManager;

// コンストラクタ
CTitleScene::CTitleScene()
	:CScene()
{
	SetSceneName("Title");
}
CTitleScene::~CTitleScene()
{

}
void CTitleScene::Init()
{
	CScene::Init();

}
void CTitleScene::Uninit()
{
	CScene::Uninit();
}
void CTitleScene::Update()
{
	CScene::Update();
}
void CTitleScene::Draw()
{
	CScene::Draw();
}