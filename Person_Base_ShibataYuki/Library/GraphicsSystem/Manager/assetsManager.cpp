//========================================================
// [assetsManager.h]
//-------------------------
// �쐬:2022/12/22 
//-------------------------
// �΂�΂�̑f�ފ֌W����Z�߂�
//========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

using namespace MySpace::Graphics;


CAssetsManager::CAssetsManager()
{
	
}

CAssetsManager::~CAssetsManager()
{
	m_pFont->Uninit();
	m_pEffekseer->Uninit();
	//m_pShader->Uninit();
	m_pEffekseer.reset();
	m_pModel.reset();
	m_pFont.reset();
	m_pImage.reset();
}

//==========================================================
// ������
//==========================================================
HRESULT CAssetsManager::Init(Application* app)
{
	HRESULT hr = S_OK;
	
	//--- �������m��
	m_pImage = std::make_shared<CImageResourceManager>();
	m_pFont = std::make_shared<CFontTexture>();
	m_pModel = std::make_shared<CModelManager>();
	m_pEffekseer = std::make_shared<CEffekseer>();
	//m_pShader = std::make_shared<CShaderManager>();
	
	m_pApp = app;
	hr = m_pEffekseer->Init(app->GetDevice(), app->GetDeviceContext());
	//m_pShader->Init();
	hr = m_pFont->Init();
	return hr;
}

//==========================================================
// �X�V
//==========================================================
void CAssetsManager::Update()
{
	m_pEffekseer->Update();
}

//==========================================================
// �V�[���j�����ɌĂяo���A�s�v�Ȃ��̂��������
//==========================================================
void CAssetsManager::SceneReset()
{
	m_pImage->SceneUnload();
	m_pModel->SceneUnload();
	m_pEffekseer->SceneUnload();

}