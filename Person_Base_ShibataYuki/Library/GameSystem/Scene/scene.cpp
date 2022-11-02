//=========================================================
//�쐬:2022/04/20 (���j��)
// �V�[���׽ : ���N���X
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Camera/camera.h>
//#include "collision.h"

//using MyLibrary::System::CGameObjectManager;
using namespace MySpace::SceneManager;
using namespace MySpace::Game;

// �R���X�g���N�^
CScene::CScene()
	:m_SceneName("none")
{
	m_drawManager = std::make_shared<CDrawManager>();
	m_objeManager = std::make_shared<CGameObjectManager>(m_spPtr.lock());

	//CreateEmptyScene();
}
// �R���X�g���N�^
CScene::CScene(std::string name)
	:m_SceneName(name)
{
	m_drawManager = std::make_shared<CDrawManager>();
	m_objeManager = std::make_shared<CGameObjectManager>(m_spPtr.lock());

	//m_objeManager->Awake();
}
// �f�X�g���N�^
CScene::~CScene()
{
	//Uninit();

	m_objeManager.reset();
	m_drawManager.reset();
}
// ������
void CScene::Init()
{
	// �V�[���ɑ΂��ăQ�[���I�u�W�F�N�g�������n��
	//m_objeManager->Init();
}
// ���
void CScene::Uninit()
{
	// �V�[����j�������ہA�X�}�[�g�|�C���^�Ŕj�������
	m_objeManager->Uninit();
}
// �X�V
void CScene::Update()
{
	m_objeManager->Update();
}
// ��莞�Ԃ̍X�V
void CScene::FixedUpdate()
{
	m_objeManager->FixedUpdate();
}
// �`��
void CScene::Draw()
{
	m_drawManager->Update();
}
void CScene::CreateEmptyScene()
{
	if (m_objeManager)
	{
		m_objeManager->Uninit();
		m_objeManager.reset();
	}
	m_objeManager = std::make_shared<CGameObjectManager>();
	m_objeManager->CreateBasicObject();
	//m_objeManager->Init();

	m_SceneName = "empty";
}