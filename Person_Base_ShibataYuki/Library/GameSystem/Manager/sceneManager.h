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
}

namespace MySpace
{
	namespace SceneManager
	{
		using namespace MySpace::Game;
		using namespace MySpace::System;
		using MySpace::System::CAppSystem;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CComponent;
		using MySpace::Game::CInstantResourceManager;
		
		//--- クラス定義
		class CSceneManager : public CAppSystem<CSceneManager>
		{
			friend class CAppSystem<CSceneManager>;
			friend class CSingleton<CSceneManager>;
		private:
			// シリアライズ用ｸﾗｽ
			// 一時的に退避させるMementoパターン
			class CSceneData
			{
			private:
				friend cereal::access;
				template <typename Archive>
				void save(Archive & archive, std::uint32_t const version) const
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_GameObjectManager), CEREAL_NVP(m_resource));
				}
				template <typename Archive>
				void load(Archive & archive, std::uint32_t const version)
				{
					archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_GameObjectManager), CEREAL_NVP(m_resource));
				}
			public:
				std::string m_SceneName;
				std::list<std::shared_ptr<CGameObject> > m_GameObjectManager;
				CInstantResourceManager m_resource;
			};

			// エイリアス
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- メンバ変数	
			std::weak_ptr<CScene> m_pCurrentScene;
			SceneList m_pScenes;
			std::string m_currentPath;
			std::shared_ptr<CSceneTransitionDetection> m_sceneDetection;
			bool m_bTransition;

		private:
			//--- ﾒﾝﾊﾞ関数
			CSceneManager();

			// *シーン破棄時に呼び出すための関数
			// *m_bTransitionをtrue
			// *再帰呼び出しされる
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
			void FixedUpdate();
			void Draw();

			// *CGameObjectManager用
			// *component内でシーンが破棄された場合、処理を抜ける
			// *CSceneManager⇒CScene⇒CGameObjectManager(引き数でポインタ与えた方がよかった?もしくはstatic)
			bool Escape()
			{
				if (m_bTransition) { m_bTransition = false; return true; }
				return false;
			}

			// *メインシーン取得(引き数ありでSP取得)
			CScene* GetActiveScene() { return m_pCurrentScene.lock().get(); }
			// *メインシーン取得
			std::shared_ptr<CScene> GetActiveScene(int) { return m_pCurrentScene.lock(); }
			std::string GetActiveSceneName() { return m_pCurrentScene.lock()->GetSceneName(); }
			// *文字列が一致するシーン取得
			std::shared_ptr<CScene> GetSceneByName(std::string name) 
			{
				for (auto & scene : m_pScenes)
				{
					if (scene->GetSceneName() == name) 
						return scene;
				}
				return std::shared_ptr<CScene>();
			}
			// *全シーンの取得
			std::vector<std::shared_ptr<CScene>> GetAllScene() { return m_pScenes; }
			
			//--- シーンの読み込み
			// *通常のシーン切替
			std::weak_ptr<CScene> SceneTransition(std::string name);

			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *シーン生成
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}
			
			std::shared_ptr<CScene> NewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene;

				// 名前指定なし
				if(name.empty())
					pNextScene = std::make_shared<CScene>();
				else
					pNextScene = std::make_shared<CScene>(name);
				// リストへの追加
				AddSceneList(pNextScene);
				// 自分のSPを教える
				pNextScene->SetScene(pNextScene);
				
				if (!m_pCurrentScene.lock())
					m_pCurrentScene = pNextScene;
				m_sceneDetection->Call(pNextScene.get(),0);	// 生成時呼び出し
				return pNextScene;
			}

			// 現在シーンに追加
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

			// シーン破棄
			// 引き数指定で切替
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		
			// *リストに追加
			// *格納できればfalse,既に存在すればtrue
			bool AddSceneList(std::shared_ptr<CScene> add)
			{
				for (auto & scene : m_pScenes)
				{
					if (scene == add)
					{
						return false;
					}
				}
				m_pScenes.push_back(add);
				return true;
			}

			void SaveScene(const std::string filename = "none");
			bool LoadScene(const std::string path);

			//--- シーン遷移時登録関数
			// *シーン遷移時呼び出し登録
			// *戻り値:void
			// *引き数:CScene* prev,CScene* next
			template <class T>
			void SceneChanged(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_sceneDetection->Changed<T>(func, ptr);
			}
			// *シーン生成時呼び出し登録
			// *戻り値:void
			// *引き数:CScene* scene, int mode
			template <class T>
			void SceneLoaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_sceneDetection->Loaded<T>(func, ptr);
			}
			// *シーン破棄時呼び出し登録
			// *戻り値:void
			// *引き数:CScene* scene
			template <class T>
			void SceneUnloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_sceneDetection->Unloaded<T>(func, ptr);
			}

			auto GetDetection() { return m_sceneDetection.get(); }
			

#ifdef BUILD_MODE

			void ImguiDebug();

#endif // BUILED_MODE
		};
	}
}
using namespace MySpace::SceneManager;


#endif // !__GAME_SCENE_H__