//=========================================================
// [fadeController.h] 
// 作成:2022/08/20
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __FADE_CONTROLLER_COMPONENT_H__
#define __FADE_CONTROLLER_COMPONENT_H__

//--- インクルード部
#include <functional>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

class CFadeController : public CComponent
{
public:
	enum class E_FadeState
	{
		NONE,			// *@ニュートラル
		FADE_OUT_START,	// *@フェードアウト開始
		FADE_OUT,		// *@フェードアウト中(関数ポインタでは終了後)
		FADE_IN_START,	// *@フェードイン開始
		FADE_IN,		// *@フェードイン(関数ポインタでは終了後)
		WAIT,			// *@待ち状態
		MAX,
	};
private:
	std::weak_ptr<CPolygonRenderer> m_pPolygon;										// *@ポリゴンptr
	E_FadeState m_eState;															// *@状態
	float m_fFadeTime;																// *@フェード実行時間
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_pFuncMap;		// *@フェード実行時関数ポインタ
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_pLoopFuncMap;	// *@フェード実行時関数ポインタ(ループ)
	
private:
	// *登録された関数の実行
	void Call(E_FadeState state)
	{
		if (m_pLoopFuncMap[state].size() != 0)
		{// 繰り返し
			for (auto & func : m_pLoopFuncMap[state])
			{
				func();
			}
		}

		// 一回
		if (m_pFuncMap.size() == 0)
			return;

		for (auto & func : m_pFuncMap[state])
		{
			func();
		}
		m_pFuncMap[state].clear();
	}
public:
	CFadeController() {};
	CFadeController(std::shared_ptr<CGameObject> owner);
	~CFadeController() {};

	void Awake();
	void Init();
	void Update();

	// *@フェードアウト開始
	void StartFadeOut();
	// *@フェードイン開始
	void StartFadeIn();

	// *@呼び出しタイミング指定
	// *@引き数:void(class)(void)型関数ポインタ, インスタンスポインタ, (bool型追加で破棄されない)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr)
	{
		m_pFuncMap[state].push_back(std::bind(func, ptr));
	}
	// *@呼び出しタイミング指定
	// *@引き数:void(class)(void)型関数ポインタ, インスタンスポインタ, (bool型追加で破棄されない)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr, bool flg)
	{
		m_pLoopFuncMap[state].push_back(std::bind(func, ptr));
	}

	//--- ゲッター・セッター

	// *@fade状況取得
	E_FadeState GetState() { return m_eState; }
	// *@動いている状態か確認
	// *@fade中ならtrue
	bool IsFadeProcess() { return (m_eState != E_FadeState::NONE); }
};


#endif // !__FADECONTROLLER_COMPONENT_H__
