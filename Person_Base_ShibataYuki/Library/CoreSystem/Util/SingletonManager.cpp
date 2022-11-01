//=========================================================
// [SingletonManager.h] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <CoreSystem/Util/SingletonManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;

//--- 定数定義

//--- 静的メンバ変数

void CSingletonManager::Create()
{
	/*Application::Start();
	CDXDevice::Start();
	CDXTexture::Start();
	CImageResourceManager::Start();
	CSceneManager::Start();
	CWindow::Start();*/
}

void CSingletonManager::Destroy()
{
	//Application::Get().ShutDown();
	//CDXDevice::Get().ShutDown();
	////CDXTexture::Get().ShutDown();
	//CFuncManager::Get().ShutDown();
	//CImageResourceManager::Get().ShutDown();
	//CSceneManager::Get().ShutDown();
	//CWindow::Get().ShutDown();
	//CImageResourceManager::Get().ShutDown();
	//CModelManager::Get().ShutDown();
	//CEffekseer::Get().ShutDown();
}