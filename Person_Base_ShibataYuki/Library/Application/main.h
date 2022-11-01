//=========================================================
// [main.h]
// �萔��`�ȂǁA���C���ōs�������̋��n��
// 2022/06/07 �X�V
// ���Z�q�̃I�[�o�[���[�h����
// unity�ۂ��׽��
//=========================================================
#ifndef __MAIN_H__
#define __MAIN_H__

#define NOMINMAX
//--- �C���N���[�h��
#include <Windows.h>
#include <CoreSystem/Math/MyMath.h>
//#include <CoreSystem/Util/define.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define MODE		1

#if MODE
//#pragma comment(lib, "Application")
//#pragma comment(lib, "CoreSystem")
//#pragma comment(lib, "DebugSystem")
//#pragma comment(lib, "GameSystem")
//#pragma comment(lib, "GraphicsSystem")


// *�J�n
HRESULT StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
// *�I��
void ShutDown();
// *�X�V
void MainLoop();

#endif // MODE

#endif // !__MAIN_H__

