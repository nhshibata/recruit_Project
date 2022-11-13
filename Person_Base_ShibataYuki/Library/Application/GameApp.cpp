//=========================================================
//�쐬:2022/05/24
// �Q�[�����s�׽
//=========================================================

//--- �C���N���[�h��
#define NOMINMAX

#include <Application/GameApp.h>
#include <Application/Application.h>
#include <Application/screen.h>
#include <Application/systemManager.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Scene/titleScene.h>
#include <GameSystem/Scene/buildScene.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/GameObject/tag.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imguiManager.h>
#include <DebugSystem/typeSaveManager.h>
#include <CoreSystem/system.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Game;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;

// �R���X�g���N�^
CGameApp::CGameApp()
{
}
// �f�X�g���N�^
CGameApp::~CGameApp()
{
}
// ������
void CGameApp::Init(Application& pApp)
{
	HWND hWnd = pApp.GetHWnd();

	// �f�o�C�X�̏�����
	auto pDX = &CDXDevice::Get();
	pDX->Init(hWnd, (unsigned int)CScreen::GetWidth(), (unsigned int)CScreen::GetHeight());
	auto pD = pDX->GetDevice();
	auto pDc = pDX->GetDeviceContext();

#ifdef BUILD_MODE
	// imGui�̏���������
	auto imgui = &ImGuiManager::Get();
	imgui->Init(hWnd, pD, pDc);
#endif // BUILD_MODE
	
	// ���͂̏�����
	CInput::Init();
	CGamePad::Init();
	Mouse::Initialize();
	Keyboad::Update();

	// ���b�V��
	CMesh::InitShader();

	// ���s�����̏�����
	// �|�X�g�G�t�F�N�g?
	// �X�J�C�h�[��

	// �V�[���̐���
	auto scene = &CSceneManager::Get();
	//CSceneManager::Get()->Init();
	//auto scene = CSceneManager::Get()->CreateNewScene<CScene>("Title");
	//CSceneManager::Get()->CreateNewScene<CScene>("Title");
	scene->Init();
	scene->CreateNewScene<CScene>("Title");

	// ���f��
	auto model = &CModelManager::Get();
	model->Init();

	// �|���S��
	CPolygon::Init(pD);

	// ��ׂ̐�����
	auto effect = &CEffekseer::Get();
	effect->Init(pD, pDc);

	// �t�H���g
	auto font = &CFontTexture::Get();
	font->Init();

	// �V�F�[�_�[
	//CShaderManager::Get()->Init();

	// ��������
	CSound::Init();

	// ��������̂��ߊi�[���Ă���
	/*pApp.MainSystem.Get()->SetSystem(ESystems::TAG, &CTagName::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::TWEEN, &CTweenManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::IMAGE, &CImageResourceManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::TYPE_SAVE, &CTypeSaveManager::Get());
	pApp.MainSystem.Get()->SetSystem(ESystems::FUNC_PTR, &CFuncManager::Get());*/

}
// �������
void CGameApp::Uninit(Application& pApp)const
{
	// ���I��
	CSound::Fin();

	// �`��֌W
	CMesh::FinShader();
	CPolygon::Fin();

	CFontTexture::Get().Uninit();

	CModelManager::Get().Uninit();

	CSceneManager::Get().Uninit();
	//CShaderManager::Get()->Uninit();
	
	// ����
	CInput::Fin();
	CGamePad::Uninit();
}
// �ʏ�X�V
void CGameApp::Update(Application& pApp)const
{
	// ���X�V
	CSound::Update();

#ifdef BUILD_MODE	// ImGui
	auto imgui = &ImGuiManager::Get();
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

	CSceneManager::Get().Update();
	CEffekseer::Get().Update();
}
// ����X�V
void CGameApp::FixedUpdate(Application& pApp)const
{
	// ��莞�Ԃ̍X�V
	CSceneManager::Get().FixedUpdate();
}
// ���͍X�V
void CGameApp::InputUpdate()
{
	CInput::Update();
	CGamePad::Update();		// �ްѓ���	
	Mouse::Update();

	Keyboad::Update();
}
// �`��
void CGameApp::Draw(Application& pApp)
{
	// �`�揀��
	BeginRender(pApp);

	// ���C�g�I�u�W�F�N�g�����݂��Ȃ��Ƃ�
	if (CCamera::GetMain())
	{
		// �V�[���̕`��
		CSceneManager::Get().Draw();

		// effect
		CSceneManager::Get().Draw();
	}

#ifdef BUILD_MODE
	// ImGui�̕`��
	ImGuiManager::Get().Render();
#endif // BUILD_MODE

	// �`���X�V
	EndRender(pApp);
}
// *@�`��O
void CGameApp::BeginRender(Application& pApp)
{
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	auto pDX = &CDXDevice::Get();
	ID3D11DeviceContext* pDC = pDX->GetDeviceContext();
	pDC->ClearRenderTargetView(pDX->GetRenderTargetView(), ClearColor);
	pDC->ClearDepthStencilView(pDX->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView* pViews[] = {
		pDX->GetRenderTargetView()
	};
	pDC->OMSetRenderTargets(1, pViews, nullptr);

	CCamera::GetMain()->DrawSkyDome();
}
// *@�`���
void CGameApp::EndRender(Application& pApp)
{
	// �`���X�V
	auto pDX = &CDXDevice::Get();
	pDX->SetCullMode((int)ECullMode::CULLMODE_NONE);
	pDX->GetSwapChain()->Present(0, 0);
}