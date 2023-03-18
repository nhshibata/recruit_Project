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
#include <DebugSystem/errorMessage.h>

using namespace MySpace::Graphics;

//=========================================================
// コンストラクタ
//=========================================================
CAssetsManager::CAssetsManager()
{
	
}

//=========================================================
// デスクトラクタ
//=========================================================
CAssetsManager::~CAssetsManager()
{
	m_pFont->Uninit();
	m_pEffekseer->Uninit();
	m_pShader->Uninit();
	m_pEffekseer.reset();
	m_pModel.reset();
	m_pFont.reset();
	m_pImage.reset();
}

//==========================================================
// 初期化
//==========================================================
HRESULT CAssetsManager::Init(Application* app)
{
	HRESULT hr = S_OK;
	
	//--- メモリ確保
	m_pImage = std::make_shared<CImageResourceManager>();
	m_pFont = std::make_shared<CFontTexture>();
	m_pModel = std::make_shared<CModelManager>();
	m_pEffekseer = std::make_shared<CEffekseer>();
	m_pShader = std::make_shared<CShaderManager>();
	
	m_pApp = app;
	//hr = m_pEffekseer->Init(app->GetDevice(), app->GetDeviceContext());

	//--- シェーダーなどの初期化
	hr = m_pShader->Init();
	if (FAILED(hr))
	{
		Debug::CErrorMessage::DispErrorHandle(hr);
		return hr;
	}
	hr = m_pFont->Init();
	if (FAILED(hr))
	{
		Debug::CErrorMessage::DispErrorHandle(hr);
		return hr;
	}

	return hr;
}

//==========================================================
// 更新
//==========================================================
void CAssetsManager::Update()
{
	//m_pShader->Update();
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

CAssetsManager* CAssetsManager::Get()
{
	static CAssetsManager instance;
	return &instance;
}