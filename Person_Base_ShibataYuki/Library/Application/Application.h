

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//--- インクルード部
#define NOMINMAX
#include <windows.h>
#include <windef.h>
#include <d3d11.h>

#include <cinttypes>
#include <stdio.h>
#include <map>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/property.h>

//#pragma comment(lib, "winmm.lib")	// winmm.libを使用する

namespace MySpace
{
	namespace System
	{
		class CSystemManager;
	}
}
using MySpace::System::CSingleton;

//--- クラス定義
class Application : public CSingleton<Application>
{
	friend class CSingleton<Application>; // Singleton でのインスタンス作成は許可
private:
	//--- メンバ変数
	HWND					m_hWnd;							// Windowハンドル
	HINSTANCE				m_hInst;						// インスタンスハンドル
	uint32_t				m_SystemCounter;				// システムカウンタ
	FILE*					m_fp;							// デバッグ用コンソール

private:
	//--- メンバ関数
	Application() { };											// コンストラクタ

	//Application(const Application&);					// コピー
	//Application& operator = (const Application&) {}		// =

public:
	//~Application() {};								// デストラクタ

	bool Init(HINSTANCE h_cpInstance);					// システム有効化
	
	void Uninit();										// システム無効化
	unsigned long MainLoop();							// メインループ

	// メンバ取得関数
	inline HWND GetHWnd() { return m_hWnd; };	// ウィンドウハンドル
	inline HINSTANCE GetHInst() { return m_hInst; };	// インスタンスハンドル
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

};

#endif // !__APPLICATION_H__