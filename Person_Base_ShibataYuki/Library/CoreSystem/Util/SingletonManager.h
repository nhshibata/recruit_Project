//=========================================================
// [SingletonManager.h] 
// 作成:2022/06/19
//---------------------------------------------------------
// Singletonをまとめたもの
// 想定と違った
// FIXME: Data にまとめるべき
//=========================================================

//--- インクルードガード
#ifndef __SINGLETON_MANAGER_H__
#define __SINGLETON_MANAGER_H__

//--- インクルード部
#include <CoreSystem/Singleton.h>

#include <Application/Application.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <GameSystem/Manager/sceneManager.h>
#include <Application/window.h>
#include <DebugSystem/imguiManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/effectManager.h>

// no
//#include "componentFactory.h"

//--- 前方参照

//--- クラス定義
namespace MySpace
{
	namespace System
	{
		class CSingletonManager
		{
		private:

		public:
			static void Create();
			static void Destroy();
		};
	}
}
#endif // !__SINGLETON_MANAGER_H__