//=========================================================
//作成:2022/06/17
//---------------------------------------------------------
// TODO: シーン切替時の処理実装必須
//=========================================================

//--- インクルードガード
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//--- インクルード部
#include <CoreSystem/system.h>
#include <CoreSystem/Util/cerealCommon.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/instantResourceManager.h>
#include <GameSystem/Scene/sceneTransitionDetection.h>
//
#include <DebugSystem/debug.h>

namespace MySpace
{
	namespace Game 
	{
		class CCollisionSystem;
		class CDrawSystem;
	}
}

namespace AI
{
	class CNavMeshBake;
}

namespace MySpace
{
	namespace SceneManager
	{
		using namespace MySpace::Game;
		using namespace MySpace::System;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CComponent;
		using MySpace::Game::CInstantResourceManager;
		using MySpace::Game::CCollisionSystem;
		using AI::CNavMeshBake;
		
		//--- クラス定義
		class CSceneManager : public CSingleton<CSceneManager>
		{
			friend class CSingleton<CSceneManager>;
		private:
			// シリアライズ用ｸﾗｽ
#pragma region Cereal
			// 一時的に退避させるMementoパターン
			class CSceneData
			{
			private:
				friend cereal::access;
				template <typename Archive>
				void save(Archive & archive, std::uint32_t const version) const
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
				}
				template <typename Archive>
				void load(Archive & archive, std::uint32_t const version)
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
				}
			public:
				std::string m_SceneName;
				std::list<std::shared_ptr<CGameObject> > m_aGameObjectManager;
				CInstantResourceManager m_resource;
			};
#pragma endregion

			//--- エイリアス
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- メンバ変数	
			std::weak_ptr<CScene> m_pCurrentScene;
			SceneList m_aScenes;
			std::string m_currentPath;
			std::shared_ptr<CSceneTransitionDetection> m_sceneDetection;

			std::shared_ptr<CCollisionSystem> m_pCollisionSystem;
			std::shared_ptr<CDrawSystem> m_pDrawSystem;
			std::shared_ptr<CNavMeshBake> m_pNavMesh;
			bool m_bTransition;

		private:
			//--- ﾒﾝﾊﾞ関数
			CSceneManager();

			// *@シーン破棄時に呼び出すための関数
			// *@m_bTransitionをtrue
			// *@再帰呼び出しされる
			void* FlagOn(CScene* scene)
			{
				m_bTransition = true;
				CSceneManager::Get().SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
					//std::bind(&CSceneManager::FlagOn, this, std::placeholders::_1));
				return nullptr;
			}
		public:
			void Init();
			void Uninit();
			void Update();
			void FixedUpdateScene();
			void DrawScene();

			// *@CGameObjectManager用
			// *@component内でシーンが破棄された場合、処理を抜ける
			// *@CSceneManager⇒CScene⇒CGameObjectManager(引き数でポインタ与えた方がよかった?もしくはstatic)
			bool Escape()
			{
				if (m_bTransition) { m_bTransition = false; return true; }
				return false;
			}

			// *@メインシーン取得(引き数ありでSP取得)
			CScene* GetActiveScene() { return m_pCurrentScene.lock().get(); }
			// *@メインシーン取得
			std::shared_ptr<CScene> GetActiveScene(int) { return m_pCurrentScene.lock(); }
			// *@メインシーン名前取得
			std::string GetActiveSceneName() { return m_pCurrentScene.lock()->GetSceneName(); }
			// *@文字列が一致するシーン取得
			std::shared_ptr<CScene> GetSceneByName(std::string name) 
			{
				for (auto & scene : m_aScenes)
				{
					if (scene->GetSceneName() == name) 
						return scene;
				}
				return std::shared_ptr<CScene>();
			}
			// *@全シーンの取得
			std::vector<std::shared_ptr<CScene>> GetAllScene() { return m_aScenes; }
			
			//--- シーン読み込み関連
			// *@通常のシーン切替
			std::weak_ptr<CScene> SceneTransition(std::string name);
			// *@メインシーン切替
			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *@シーン生成
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}
			
			// *@シーン生成
			std::shared_ptr<CScene> NewScene(std::string name = std::string());

			// *@現在シーンに追加
			template <class T>
			CScene* AddScene()
			{
				std::shared_ptr<CScene> sp = NewScene();
				if (auto scene = std::dynamic_pointer_cast<T>(sp); !scene)
					return nullptr;
				else {
					AddSceneList(scene);
					return scene.get();
				}
				AddSceneList(sp);
				//sp->StartUp();
				return sp.get();
			}

			// *@シーン破棄
			// *@引き数指定で切替
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		
			// *@リストに追加
			// *@格納できればfalse,既に存在すればtrue
			bool AddSceneList(std::shared_ptr<CScene> add)
			{
				for (auto & scene : m_aScenes)
				{
					if (scene == add)
					{
						return false;
					}
				}
				m_aScenes.push_back(add);
				return true;
			}

			// *@セーブ
			void SaveScene(const std::string filename = "none");
			// *@ロード
			bool LoadScene(const std::string path);

			//--- シーン遷移時登録関数
			// *@シーン遷移時呼び出し登録
			// *@戻り値:void
			// *@引き数:CScene* prev,CScene* next
			template <class T>
			void SceneChanged(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_sceneDetection->Changed<T>(func, ptr);
			}
			// *@シーン生成時呼び出し登録
			// *@戻り値:void
			// *@引き数:CScene* scene, int mode
			template <class T>
			void SceneLoaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_sceneDetection->Loaded<T>(func, ptr);
			}
			// *@シーン破棄時呼び出し登録
			// *@戻り値:void
			// *@引き数:CScene* scene
			template <class T>
			void SceneUnloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_sceneDetection->Unloaded<T>(func, ptr);
			}

			// *@切替呼び出しｸﾗｽの取得
			inline CSceneTransitionDetection* GetDetection() { return m_sceneDetection.get(); }
			
			// *@コリジョンシステムの取得
			inline std::shared_ptr<CCollisionSystem> GetCollisionSystem() { return m_pCollisionSystem; }
			
			// *@描画システムの取得
			inline std::shared_ptr<CDrawSystem> GetDrawSystem() { return m_pDrawSystem; }

			// *@所持NavMeshの取得
			inline std::shared_ptr<CNavMeshBake> GetNavMesh() { return m_pNavMesh; };

#ifdef BUILD_MODE
		private:
			char m_cDebugSceneName[256] = "none";
		public:
			void ImGuiDebug();

#endif // BUILED_MODE
		};
	}
}
using namespace MySpace::SceneManager;


#endif // !__GAME_SCENE_H__