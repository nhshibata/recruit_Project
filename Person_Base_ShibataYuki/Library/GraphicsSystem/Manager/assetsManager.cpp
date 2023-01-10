//========================================================
// [assetsManager.h]
//-------------------------
// 作成:2022/12/22 
//-------------------------
// ばらばらの素材関係を一纏めに
//========================================================

//--- インクルード部
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
	m_pImage = std::make_shared<CImageResourceManager>();
	m_pFont = std::make_shared<CFontTexture>();
	m_pModel = std::make_shared<CModelManager>();
	m_pEffekseer = std::make_shared<CEffekseer>();
	//m_pShader = std::make_shared<CShaderManager>();
}

CAssetsManager::~CAssetsManager()
{
	m_pFont->Uninit();
	m_pEffekseer->Uninit();
	//m_pShader->Uninit();

}

//==========================================================
// 初期化
//==========================================================
void CAssetsManager::Init(Application* app)
{
	m_pApp = app;
	m_pEffekseer->Init(app->GetDevice(), app->GetDeviceContext());
	//m_pShader->Init();
	m_pFont->Init();
}

//==========================================================
// 更新
//==========================================================
void CAssetsManager::Update()
{
	m_pEffekseer->Update();
}

//==========================================================
// シーン破棄時に呼び出し、不要なものを解放する
//==========================================================
void CAssetsManager::SceneReset()
{
	m_pImage->SceneUnload();
	m_pModel->SceneUnload();
	m_pEffekseer->SceneUnload();

}