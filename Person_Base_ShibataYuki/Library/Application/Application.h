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
#include <CoreSystem/Util/define.h>


using MySpace::System::CSingleton;

//--- クラス定義
class Application : public CSingleton<Application>
{
	friend class CSingleton<Application>; // Singleton でのインスタンス作成は許可
private:
	//--- メンバ変数
	HWND					m_hWnd;						// Windowハンドル
	HINSTANCE				m_hInst;					// インスタンスハンドル
	std::map<std::string, void*>	m_aSystems;

private:
	//--- メンバ関数
	Application() = default;							// コンストラクタ
	~Application() = default;							// デストラクタ

public:
	bool Init(HINSTANCE h_cpInstance);					// システム有効化
	
	void Uninit();										// システム無効化
	unsigned long MainLoop();							// メインループ

	//--- ゲッター・セッター
	inline HWND GetHWnd() { return m_hWnd; };			// ウィンドウハンドル
	inline HINSTANCE GetHInst() { return m_hInst; };	// インスタンスハンドル
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// *@システムへの生ﾎﾟｲﾝﾀによる追加
	template <class T>
	T* AddSystem()
	{
		std::string name = typeid(T).name();
		if (m_aSystems.count(name))
		{
			return m_aSystems[name];
		}

		m_aSystems[name] = new T;
		return m_aSystems[name];
	}

	// *@型指定によるﾎﾟｲﾝﾀの取得
	template <class T>
	T* GetSystem()
	{
		std::string name = typeid(T).name();
		if (!m_aSystems.count(name))
			return nullptr;
		T* ret = static_cast<T*>(m_aSystems[name]);
		return ret;
	}
};

#endif // !__APPLICATION_H__