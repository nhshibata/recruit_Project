//=========================================================
//�쐬:2022/04/20 (���j��)
// �V�[���׽ : �h���N���X
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Scene/buildScene.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <DebugSystem/imguiManager.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Debug;

// �R���X�g���N�^
CBuildScene::CBuildScene()
	:CScene("Build")
{
	Init();
}
CBuildScene::CBuildScene(std::string name)
	:CScene(name)
{

}
CBuildScene::~CBuildScene()
{
}
void CBuildScene::Init()
{
	// Build�\�z�ɕK�v�Ȋ֐����Ăяo��
	auto obj = m_objeManager->CreateGameObject();
	obj->AddComponent<CDebugCamera>();

	obj = m_objeManager->CreateGameObject();
	obj->AddComponent<CDirectionalLight>();

	CScene::Init();

	// �V�[���ǂݍ���
	//CSceneManager::Get().LoadScene(FORDER_DIR(Data/scene/build2.scene));

	// �|�[�Y���
	//Debug::ImGuiManager::Get().SetPause(true);
	//for(this->GetObjManager()->GetList());
}
void CBuildScene::Uninit()
{
	CScene::Uninit();
}
void CBuildScene::Update()
{
	CScene::Update();
}
void CBuildScene::Draw()
{
	CScene::Draw();
}