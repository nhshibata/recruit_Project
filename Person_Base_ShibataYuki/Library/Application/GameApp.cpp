//=========================================================
// [GameApp.cpp]
//---------------------------------------------------------
//�쐬:2022/05/24
//---------------------------------------------------------
// �Q�[�����s�׽
//=========================================================

#define NOMINMAX

//--- �C���N���[�h��
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
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
//#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>

#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

#include <tchar.h>

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
HRESULT CGameApp::Init(Application* app)
{
	HRESULT hr = S_OK;

	//--- �ϐ��錾
	auto pDevice = app->GetDevice();
	auto pDC = app->GetDeviceContext();

	//--- ���͏�����
	hr = CInput::Init();
	if(hr != S_OK)
		MessageBox(NULL, _T("CInput�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
	CGamePad::Init();
	Mouse::Initialize();
	//Keyboad::();

	//--- ���b�V��
	hr = CMesh::InitShader();
	if (hr != S_OK)
		MessageBox(NULL, _T("CMesh�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
	//--- ���f��
	hr = CAssimpModel::InitShader(pDevice) ? S_OK : S_FALSE;
	if (hr != S_OK)
		MessageBox(NULL, _T("CAssimpModel�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
	//--- �|���S��
	hr = CPolygon::InitShader(pDevice);
	if (hr != S_OK)
		MessageBox(NULL, _T("CPolygon�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);

	//--- �A�Z�b�g
	// �f�ޑS�ʏ����׽
	auto pAssets = CAssetsManager::Get();
	app->AddSystem(pAssets, typeid(CAssetsManager).name());
	hr = pAssets->Init(app);
	if (hr != S_OK)
		MessageBox(NULL, _T("CAssetsManager�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);

	// ��������
	CSound::Init();

	//--- �V�[���̐���
	{
		auto sceneMgr = CSceneManager::Get();
		hr = sceneMgr->Init();
		if (hr != S_OK)
			MessageBox(NULL, _T("CSceneManager�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
	}

	//--- Effekseer
	hr = pAssets->GetEffekseer()->Init(pDevice, pDC);
	if (hr != S_OK)
		MessageBox(NULL, _T("CEffekseer�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);

#ifdef BUILD_MODE
	//--- imGui�̏���������
	auto imgui = ImGuiManager::Get();
	app->AddSystem(imgui, typeid(ImGuiManager).name());
	hr = imgui->Init(Application::Get()->GetHWnd(), pDevice, pDC);
	if (hr != S_OK)
		MessageBox(NULL, _T("ImGui�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif // BUILD_MODE

	//app->AddSystem<CTweenManager>();
	return hr;
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
		auto all = CSceneManager::Get()->GetAllScene();
		for (auto & scene : all)
		{
			scene->GetObjManager()->UpdateInDebug();
		}
		return;
	}
#endif // DEBUG

	//--- �V�[���X�V
	CSceneManager::Get()->UpdateScene();

	// Tween�̍X�V(���Ԍ���)
	//app->GetSystem<CTweenManager>()->Update();

	app->GetSystem<CAssetsManager>()->Update();
}

//==========================================================
// ��莞�Ԃ̍X�V
//==========================================================
void CGameApp::FixedUpdate(Application* app)const
{
	CSceneManager::Get()->FixedUpdateScene();
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
	//--- �`��N���A
	BeginRender(app);

#ifdef BUILD_MODE
	auto imgui = app->GetSystem<ImGuiManager>();

	//--- �`���ύX
	if (imgui->IsSceneRender())
	{
		imgui->SceneRenderClear();
		imgui->SceneRender();
	}
	else
	{
		auto pDX = app->GetSystem<CDXDevice>();
		pDX->SwitchRender(pDX->GetRenderTargetView(), pDX->GetDepthStencilView());
	}

	// �I�u�W�F�N�g�����݂��Ȃ��Ƃ�
	if (CCamera::GetMain() && CLight::GetMain())
	{
		// �X�J�C�h�[���`��
		CCamera::GetMain()->DrawSkyDome();
		
		// �V�[���̕`��
		CSceneManager::Get()->DrawScene();

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
		CSceneManager::Get()->DrawScene();

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
	//--- Ҳ݉�ʃN���A
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};
	pDC->OMSetRenderTargets(1, pViews, pDX->GetDepthStencilView());*/
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