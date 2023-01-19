//=========================================================
// [sceneManager.h]
//---------------------------------------------------------
// 作成:2022/06/17
//---------------------------------------------------------
// 
//=========================================================

//--- インクルードガード
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//--- インクルード部
#include <winerror.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/instantResourceManager.h>
#include <GameSystem/Scene/sceneTransitionDetection.h>

#pragma region ForwardDeclaration
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
#pragma endregion


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
		class CSceneManager
		{
		private:
			// シリアライズ用ｸﾗｽ
#pragma region Cereal
			// 一時的に退避させるMementoパターン?
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
				std::list<std::shared_ptr<CGameObject>> m_aGameObjectManager;
				CInstantResourceManager m_resource;
			};
#pragma endregion

		private:
			//--- エイリアス
			using SceneList = std::vector<std::shared_ptr<CScene>>;

		private:
			//--- メンバ変数	
			std::weak_ptr<CScene> m_pCurrentScene;
			SceneList m_aScenes;
			std::string m_currentPath = std::string();

			std::shared_ptr<CSceneTransitionDetection> m_sceneDetection;
			std::shared_ptr<CCollisionSystem> m_pCollisionSystem;
			std::shared_ptr<CDrawSystem> m_pDrawSystem;
			std::shared_ptr<CNavMeshBake> m_pNavMesh;
			bool m_bTransition;

		private:
			//--- ﾒﾝﾊﾞ関数
			CSceneManager();
			~CSceneManager();
			
			// *@シーン破棄時に呼び出すための関数
			// *@m_bTransitionをtrue
			// *@再帰呼び出しされる
			void* FlagOn(CScene* scene)
			{
				m_bTransition = true;
				CSceneManager::Get()->SceneUnloaded<CSceneManager>(&CSceneManager::FlagOn, this);
					//std::bind(&CSceneManager::FlagOn, this, std::placeholders::_1));
				return scene ? nullptr : nullptr;
			}

			// *@リストに追加
			// *@格納できればfalse,既に存在すればtrue
			bool AddList(std::shared_ptr<CScene> add);

		public:
			
//#pragma warning(push)
//#pragma warning(disable:4789)
			// *@シングルトンの取得
			static CSceneManager* Get();
//#pragma warning(pop)   
			
			HRESULT Init();
			void Uninit();
			void UpdateScene();
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

			//--- シーン読み込み関連
			
			// *@セーブ
			void SaveScene(const std::string filename = "none");

			// *@ロード
			bool LoadScene(const std::string path);

			// *@スタート
			void SetStartScene(std::shared_ptr<CScene> scene)
			{
				m_pCurrentScene = scene;
				m_aScenes.push_back(scene);
			}

			// *@通常のシーン切替
			std::weak_ptr<CScene> SceneTransition(std::string name);
			
			// *@メインシーン切替
			std::weak_ptr<CScene> SetActiveScene(std::shared_ptr<CScene> pNextScene);

			// *@シーン生成
			std::shared_ptr<CScene> NewScene(std::string name = std::string());
			
			// *@シーン生成
			template <class T = CScene>
			CScene* CreateNewScene(std::string name = std::string())
			{
				std::shared_ptr<CScene> pNextScene = std::dynamic_pointer_cast<T>(NewScene(name));
				if(m_pCurrentScene.lock() != pNextScene)
					RemoveScene(m_pCurrentScene.lock(), pNextScene);
			
				return m_pCurrentScene.lock().get();
			}

			// *@シーン破棄
			// *@引き数指定で切替
			void RemoveScene(std::shared_ptr<CScene> pRemove, std::shared_ptr<CScene> pNext = std::shared_ptr<CScene>());
		

			//--- シーン遷移時登録関数
			
			// *@シーン遷移時呼び出し登録
			// *@戻り値:void*
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

			//--- ゲッター

			// *@文字列が一致するシーン取得
			std::shared_ptr<CScene> GetSceneByName(std::string name);

			// *@メインシーン取得(引き数ありでSP取得)
			CScene* GetActiveScene()const										{ return m_pCurrentScene.lock().get(); }

			// *@メインシーン取得(引き数なしで生ﾎﾟｲﾝﾀ取得)
			std::shared_ptr<CScene> GetActiveScene(int)const					{ return m_pCurrentScene.lock(); }

			// *@メインシーン名前取得
			std::string GetActiveSceneName()const								{ return m_pCurrentScene.lock()->GetSceneName(); }

			// *@全シーン取得
			std::vector<std::shared_ptr<CScene>> GetAllScene()const				{ return m_aScenes; }

			// *@切替呼び出しｸﾗｽの取得
			inline CSceneTransitionDetection* GetDetection()const				{ return m_sceneDetection.get(); }
			
			// *@コリジョンシステムの取得
			inline std::shared_ptr<CCollisionSystem> GetCollisionSystem()const	{ return m_pCollisionSystem; }
			
			// *@描画システムの取得
			inline std::shared_ptr<CDrawSystem> GetDrawSystem()const			{ return m_pDrawSystem; }

			// *@所持NavMeshの取得
			inline std::shared_ptr<CNavMeshBake> GetNavMesh()const				{ return m_pNavMesh; };

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