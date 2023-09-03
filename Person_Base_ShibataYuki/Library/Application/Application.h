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
#include <CoreSystem/systemBase.h>
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
	std::map<std::string, MySpace::System::CSystemBase*>	m_aSystems;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	bool m_bIsRun;

private:
	//--- メンバ関数
	Application();										// コンストラクタ
	~Application();										// デストラクタ

public:
	bool Init(HINSTANCE h_cpInstance);					// システム有効化
	
	void Destroy();										// システム無効化
	unsigned long MainLoop();							// メインループ

	//--- ゲッター・セッター
	inline HWND GetHWnd() { return m_hWnd; };			// ウィンドウハンドル
	inline HINSTANCE GetHInst() { return m_hInst; };	// インスタンスハンドル
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	// *@終了
	void Quit() { m_bIsRun = false; }

	// *@システムへの生ﾎﾟｲﾝﾀによる追加
	MySpace::System::CSystemBase* AddSystem(MySpace::System::CSystemBase* add, std::string name)
	{
		if (m_aSystems.count(name))
		{
			return m_aSystems[name];
		}

		m_aSystems.insert(std::make_pair(name, add));
		return add;
	}

	// *@型指定によるﾎﾟｲﾝﾀの取得
	template <class T>
	T* GetSystem()
	{
		std::string name = typeid(T).name();
		if (!m_aSystems.count(name))
			return nullptr;
		T* ret = dynamic_cast<T*>(m_aSystems[name]);
		return ret;
	}
};

#endif // !__APPLICATION_H__