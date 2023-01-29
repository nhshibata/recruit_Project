//=========================================================
// [gameScene.h] 
// 作成:2022/08/18
//---------------------------------------------------------
//=========================================================

#ifndef __GAME_CETIPEDE_MARCH_COMPONENT_H__
#define __GAME_CETIPEDE_MARCH_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/Manager/sceneManager.h>

namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
	namespace Game
	{
		class CDebugCamera;
		class CTextRenderer;
	}
}

//--- クラス定義
/*
 UEを見てGameInstanceの便利さを見て真似
 非破壊登録を行う
*/
class CFadeController;
class CMyCamera;
class CGameCentipedeMarch : public CComponent
{
private:
	bool m_bPause;
	std::weak_ptr<CFadeController> m_pFade;
	std::weak_ptr<CDebugCamera> m_pDebugCamera;
	std::weak_ptr<CMyCamera> m_pMyCamera;
	int m_nResultNum;
	int m_nMaxTime = 120;

public:
	CGameCentipedeMarch() {};
	CGameCentipedeMarch(std::shared_ptr<CGameObject> owner);
	~CGameCentipedeMarch() {};

	void Awake();
	void Init();
	void Update();
	void FixedUpdate();
	void Pause();
	void FadeFuncPtr(void);
	void ObjStateSwitch(CGameObject::E_ObjectState state, CScene* scene);

	void* CreateManager(CScene* prev, CScene* next);
	int GetResultNum() { return m_nResultNum; }
	void SetResultNum(int n) { m_nResultNum = n; }
	int GetGameTime() { return m_nMaxTime; }
	void SetGameTime(int time) { m_nMaxTime = time; }
};

class CSceneManagerComponent : public CComponent
{
protected:
	std::weak_ptr<CFadeController> m_pFade;
	std::string m_NextSceneName;
public:
	CSceneManagerComponent() {};
	CSceneManagerComponent(std::shared_ptr<CGameObject> owner, std::string name = std::string()) 
		:CComponent(owner),m_NextSceneName(name)
	{};
	~CSceneManagerComponent() {};

	virtual void Init();
	void FuncPtr();
	virtual void SetScene(std::string next)final;
	void SetNextSceneName(std::string name) { m_NextSceneName = name; }
};
class CTitleSceneManager : public CSceneManagerComponent
{
public:
	static inline constexpr const char* SCENE_NAME = "Tilte";
	static inline constexpr const char* NEXT_SCENE_NAME = "Game";
private:
	std::weak_ptr<CTextRenderer> m_pNum;
	std::weak_ptr<CGameCentipedeMarch> m_pGameManager;

public:
	CTitleSceneManager() {};
	CTitleSceneManager(std::shared_ptr<CGameObject> owner) :CSceneManagerComponent(owner, NEXT_SCENE_NAME)
	{
	};
	~CTitleSceneManager() {};

	void Awake();
	void Init();
	void Update(); 
};
class CTimer;
class CCentipede;
class CGameSceneManager : public CSceneManagerComponent
{
public:
	static inline constexpr const char* SCENE_NAME = "Game";
	static inline constexpr const char* NEXT_SCENE_NAME = "Result";
private:
	std::weak_ptr<CTimer> m_pTimer;
	std::weak_ptr<CCentipede> m_pPlayer;
	//std::weak_ptr<CMyCamera> m_pMyCamera;
	bool m_bEnd = false;
	int m_nSize = 0;

public:
	CGameSceneManager() {};
	CGameSceneManager(std::shared_ptr<CGameObject> owner) :CSceneManagerComponent(owner, NEXT_SCENE_NAME)
	{
	};
	~CGameSceneManager() {};

	void Awake();
	void Init();
	void Update();
};
class CResultSceneManager : public CSceneManagerComponent
{
public:
	static inline constexpr const char* SCENE_NAME = "Result";
	static inline constexpr const char* NEXT_SCENE_NAME = "Title";

public:
	CResultSceneManager() {};
	CResultSceneManager(std::shared_ptr<CGameObject> owner) :CSceneManagerComponent(owner, NEXT_SCENE_NAME)
	{
	};
	~CResultSceneManager() {};

	void Awake();
	void Init();
	void Update();
};

class CTutrialSceneManager : public CSceneManagerComponent
{
public:
	static inline constexpr const char* SCENE_NAME = "Tutrial";
	static inline constexpr const char* NEXT_SCENE_NAME = "Game";
private:
	std::weak_ptr<CTimer> m_pTimer;
	std::weak_ptr<CCentipede> m_pPlayer;
	//std::weak_ptr<CMyCamera> m_pMyCamera;
	bool m_bEnd = false;
	int m_nSize = 0;
public:
	CTutrialSceneManager() {};
	CTutrialSceneManager(std::shared_ptr<CGameObject> owner) :CSceneManagerComponent(owner, NEXT_SCENE_NAME)
	{
	};
	~CTutrialSceneManager() {};

	void Awake();
	void Init();
	void Update();
};

//recycle
//template <class T = CComponent>
//class CRecycle : public CComponent
//{
//private:
//	std::vector<std::weak_ptr<T>> m_pActiveList;
//	std::vector<std::weak_ptr<T>> m_pStandby;
//public:
//	CRecycle() {};
//	CRecycle(std::shared_ptr<CGameObject> owner)
//		:CComponent(owner)	{}
//	~CRecycle() {};
//
//	void Standby(std::weak_ptr<T> ptr)
//	{
//		m_pStandby.push_back(ptr);
//		auto it = std::find(m_pActiveList.begin(), m_pActiveList.end(), ptr);
//		m_pActiveList.erase(it);
//	}
//	void Create(Vector3 pos)
//	{
//		if (m_pStandby.size() != 0)
//		{
//			// 最後尾を再利用
//			auto item = m_pStandby.back();
//			item.lock()->SetActive(CGameObject::E_ObjectState::ACTIVE);
//			item.lock()->Transform()->SetPos(pos);
//			m_pStandby.pop_back();
//			m_pActiveList.push_back(item);
//			return;
//		}
//
//		auto obj = CGameObject::CreateObject();
//		obj.lock()->GetTransform()->SetPos(pos);
//		m_pActiveList.push_back(obj.lock()->AddComponent<CItemBase>());
//	}
//};

#endif // !__GAME_SCENE_COMPONENT_H__
