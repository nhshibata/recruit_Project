//=========================================================
// [main.h]
// 定数定義など、メインで行う処理の橋渡し
// 2022/06/07 更新
// 演算子のオーバーロード試し
// unityぽいｸﾗｽに
//=========================================================
#ifndef __MAIN_H__
#define __MAIN_H__

#define NOMINMAX
//--- インクルード部
#include <Windows.h>
#include <CoreSystem/Math/MyMath.h>
//#include <CoreSystem/Util/define.h>

//*****************************************************************************
// マクロ定義
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


// *開始
HRESULT StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
// *終了
void ShutDown();
// *更新
void MainLoop();

#endif // MODE

#endif // !__MAIN_H__

