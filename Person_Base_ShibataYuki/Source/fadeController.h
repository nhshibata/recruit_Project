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
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		class CPolygonRenderer;
	}
}

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
	std::weak_ptr<MySpace::Game::CPolygonRenderer> m_pPolygon;						// ポリゴンptr
	std::string m_SceneName;										// 切替先
	E_FadeState m_eState;											// 状態
	float m_fFadeTime;												// フェード実行時間
	
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_aFuncMap;		// フェード実行時関数ポインタ
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_aLoopFuncMap;	// フェード実行時関数ポインタ(ループ)
	
private:
	// *@登録された関数の実行
	void Call(E_FadeState state);

public:
	CFadeController();
	CFadeController(std::shared_ptr<CGameObject> owner);
	~CFadeController();

	virtual void Awake();
	virtual void Init();
	void Update();

	// *@フェードアウト後にシーン切替(シーン名を引き数指定)
	// *@切替後、フェードイン
	void SetScene(std::string sceneName);

	// *@フェードアウト開始
	void StartFadeOut();
	// *@フェードイン開始
	void StartFadeIn();

	// *@関数ポインタ設定
	// *@呼び出しタイミング指定
	// *@引き数:void(class)(void)型関数ポインタ, インスタンスポインタ, (bool型追加で破棄されない)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr)
	{
		m_aFuncMap[state].push_back(std::bind(func, ptr));
	}

	// *@関数ポインタ設定
	// *@呼び出しタイミング指定
	// *@引き数:void(class)(void)型関数ポインタ, インスタンスポインタ, (bool型追加で破棄されない)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr, bool flg)
	{
		m_aLoopFuncMap[state].push_back(std::bind(func, ptr));
	}

	//--- ゲッター・セッター

	// *@fade状況取得
	const E_FadeState GetFadeState() const { return m_eState; }
	
	// *@動いている状態か確認
	// *@fade中ならtrue
	const bool IsFadeProcess() { return (m_eState != E_FadeState::NONE); }
};


#endif // !__FADE_CONTROLLER_COMPONENT_H__
