//=========================================================
//�쐬:2022/05/24
// �Q�[�����s�׽
//=========================================================

//--- �C���N���[�h��
#define NOMINMAX

#include <Application/GameApp.h>
#include <Application/Application.h>
#include <Application/screen.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/GameObject/tag.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Manager/assetsManager.h>
//#include <GraphicsSystem/Manager/imageResourceManager.h>
//#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
//#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>

#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;


//==========================================================
// �R���X�g���N�^
//==========================================================
CGameApp::CGameApp()
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGameApp::~CGameApp()
{
}

//==========================================================
// ������
//==========================================================
void CGameApp::Init(Application* app)
{
	HWND hWnd = Application::Get()->GetHWnd();

	//--- �ϐ��錾
	auto pDevice = app->GetDevice();
	auto pDC = app->GetDeviceContext();

	//--- ���͏�����
	CInput::Init();
	CGamePad::Init();
	Mouse::Initialize();
	Keyboad::Update();

	//--- ���b�V��
	CMesh::InitShader();

	// ���f��
	CAssimpModel::InitShader(pDevice);

	//--- �|���S��
	CPolygon::Init(pDevice);

	//--- �A�Z�b�g
	// �f�ޑS�ʏ����׽
	auto pAssets = app->AddSystem<CAssetsManager>();
	pAssets->Init(app);

	// ��������
	CSound::Init();

	//--- �V�[���̐���
	auto sceneMgr = &CSceneManager::Get();
	sceneMgr->Init();
	

#ifdef BUILD_MODE
	//--- imGui�̏���������
	auto imgui = app->AddSystem<ImGuiManager>();
	if (!imgui)
	{
		int a = 0;
	}
	imgui->Init(hWnd, pDevice, pDC);
#endif // BUILD_MODE

	app->AddSystem<CTweenManager>();

}

//==========================================================
// �������
//==========================================================
void CGameApp::Uninit(Application* app)const
{
	//_CrtDumpMemoryLeaks();

	// ���I��
	CSound::Fin();

	// Assimp�p�V�F�[�_�I������
	CAssimpModel::UninitShader();

	// �`��֌W
	CMesh::FinShader();
	CPolygon::Fin();

	CTypeSaveManager::Get()->Uninit();

	// ����
	CInput::Fin();
	CGamePad::Uninit();
	_CrtDumpMemoryLeaks();

}

//==========================================================
// �ʏ�X�V
//==========================================================
void CGameApp::Run(Application* app)
{
	
	// ���X�V
	CSound::Update();

#ifdef BUILD_MODE	// ImGui
	auto imgui = app->GetSystem<ImGuiManager>();
	imgui->Update();
	
	// �f�o�b�O���̍X�V(GameObject��delete��Transform�̍X�V�͂Ȃ��ƕs��)
	if (imgui->GetPause())
	{
		auto all = CSceneManager::Get().GetAllScene();
		for (auto & scene : all)
		{
			scene->GetObjManager()->UpdateInDebug();
		}
		return;
	}
#endif // DEBUG

	CSceneManager::Get().UpdateScene();

	// Tween�̍X�V(���Ԍ���)
	app->GetSystem<CTweenManager>()->Update();

	app->GetSystem<CAssetsManager>()->Update();
}

//==========================================================
// ��莞�Ԃ̍X�V
//==========================================================
void CGameApp::FixedUpdate(Application* app)const
{
	CSceneManager::Get().FixedUpdateScene();
}

//==========================================================
// ���͍X�V
//==========================================================
void CGameApp::InputUpdate()
{
	CInput::Update();
	CGamePad::Update();		// �ްѓ���	
	Mouse::Update();

	Keyboad::Update();
}

//==========================================================
// �`��
//==========================================================
void CGameApp::Draw(Application* app)
{
	//--- �`���ݒ�
	BeginRender(app);	// �`�揀��

#ifdef BUILD_MODE
	auto imgui = app->GetSystem<ImGuiManager>();

	if (imgui->IsSceneRender())
	{
		imgui->SceneRenderClear();
		imgui->SceneRender();
	}
	else
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(nullptr, nullptr);
	}

	// �I�u�W�F�N�g�����݂��Ȃ��Ƃ�
	if (CCamera::GetMain() && CLight::GetMain())
	{
		// �X�J�C�h�[���`��
		CCamera::GetMain()->DrawSkyDome();
		
		// �V�[���̕`��
		CSceneManager::Get().DrawScene();

		// effect
		app->GetSystem<CAssetsManager>()->GetEffekseer()->Draw();
	}

	imgui->SceneGizmo();

	//--- �`���ؑ�
	if (imgui->IsSceneRender())
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(nullptr, nullptr);
	}

	//--- ImGui�̕`��
	imgui->Render();

#else

	// �I�u�W�F�N�g�����݂��Ȃ��Ƃ�
	if (CCamera::GetMain() && CLight::GetMain())
	{
		CCamera::GetMain()->DrawSkyDome();
		
		// �V�[���̕`��
		CSceneManager::Get().DrawScene();

		// effect
		app->GetSystem<CAssetsManager>()->GetEffekseer()->Draw();
	}
#endif // BUILD_MODE

	// �`���X�V
	EndRender(app);
}

//==========================================================
// �`��O
//==========================================================
void CGameApp::BeginRender(Application* app)
{
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	auto pDX = app->GetSystem<CDXDevice>();
	ID3D11DeviceContext* pDC = pDX->GetDeviceContext();
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};*/
	//pDC->OMSetRenderTargets(1, pViews, nullptr);
}

//==========================================================
// �`���
//==========================================================
void CGameApp::EndRender(Application* app)
{
	auto pDX = app->GetSystem<CDXDevice>();
	pDX->SetCullMode((int)ECullMode::CULLMODE_NONE);
	pDX->GetSwapChain()->Present(0, 0);
}