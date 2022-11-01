//=========================================================
// [gameSpellRhythm.h] 
// 作成:2022/09/20
//---------------------------------------------------------
//=========================================================

#ifndef __GAME_SPELL_RHYTHM_COMPONENT_H__
#define __GAME_SPELL_RHYTHM_COMPONENT_H__

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
namespace Spell
{
	class CGameCamera;
}

//--- クラス定義
/*
 UEを見てGameInstanceの便利さを見て真似
 非破壊登録を行う
*/
class CFadeController;

// 制作を分けるための名前空間の展開
namespace Spell
{
	class CGameSpellRhythm : public CComponent
	{
	private:
		bool m_bPause;
		std::weak_ptr<CFadeController> m_pFade;
		std::weak_ptr<CDebugCamera> m_pDebugCamera;
		std::weak_ptr<Spell::CGameCamera> m_pGameCamera;
		int m_nMaxTime = 120;

	public:
		CGameSpellRhythm() {};
		CGameSpellRhythm(std::shared_ptr<CGameObject> owner);
		~CGameSpellRhythm() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();
		void Pause();
		void FadeFuncPtr(void);
		void ObjStateSwitch(CGameObject::E_ObjectState state, CScene* scene);

		void* CreateManager(CScene* prev, CScene* next);
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
			:CComponent(owner), m_NextSceneName(name)
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
		std::weak_ptr<CGameSpellRhythm> m_pGameManager;

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
	class CGameSceneManager : public CSceneManagerComponent
	{
	public:
		static inline constexpr const char* SCENE_NAME = "Game";
		static inline constexpr const char* NEXT_SCENE_NAME = "Result";
	private:
		bool m_bEnd = false;

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
		bool m_bEnd = false;

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
}
#endif // !__GAME_SCENE_COMPONENT_H__
