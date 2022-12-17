//=========================================================
// [Application.cpp]
// ゲーム実行ｸﾗｽ
//------------------------
// 作成:2022/05/24
// 更新:2022/11/12 FixedUpdate実装
// 
//=========================================================

//--- インクルードガード
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#define NOMINMAX

//--- インクルード部
#include <windows.h>
#include <windef.h>
#include <d3d11.h>

#include <cinttypes>
#include <stdio.h>
#include <map>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/property.h>


using MySpace::System::CSingleton;

//--- クラス定義
class Application : public CSingleton<Application>
{
	friend class CSingleton<Application>; // Singleton でのインスタンス作成は許可
private:
	//--- メンバ変数
	HWND					m_hWnd;						// Windowハンドル
	HINSTANCE				m_hInst;					// インスタンスハンドル

private:
	//--- メンバ関数
	Application() {};									// コンストラクタ
	~Application() {};									// デストラクタ
public:
	bool Init(HINSTANCE h_cpInstance);					// システム有効化
	
	void Uninit();										// システム無効化
	unsigned long MainLoop();							// メインループ

	//--- ゲッター・セッター
	inline HWND GetHWnd() { return m_hWnd; };			// ウィンドウハンドル
	inline HINSTANCE GetHInst() { return m_hInst; };	// インスタンスハンドル
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

};

#endif // !__APPLICATION_H__