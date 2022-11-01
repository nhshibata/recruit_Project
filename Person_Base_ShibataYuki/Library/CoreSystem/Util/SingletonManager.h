//=========================================================
// [SingletonManager.h] 
// �쐬:2022/06/19
//---------------------------------------------------------
// Singleton���܂Ƃ߂�����
// �z��ƈ����
// FIXME: Data �ɂ܂Ƃ߂�ׂ�
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SINGLETON_MANAGER_H__
#define __SINGLETON_MANAGER_H__

//--- �C���N���[�h��
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

//--- �O���Q��

//--- �N���X��`
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