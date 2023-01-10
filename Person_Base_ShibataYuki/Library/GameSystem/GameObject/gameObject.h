//=========================================================
// [CGameObject.h]
//-----------------------
// 作成:2022/04/19 (火曜日)
//-----------------------
// ゲーム内オブジェクトクラス
// ｺﾝﾎﾟｰﾈﾝﾄを持つことが出来る、タグとレイヤーを設定できる 
//=========================================================

//--- インクルードガード
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

//--- インクルード部
#include <GameSystem/GameObject/BaseObject.h>
#include <GameSystem/GameObject/tag.h>
#include <GameSystem/GameObject/layer.h>
//#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		//--- 前方参照
		class CComponent;
		class CTransform;
		class CRenderer;
	}
}
namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}

namespace MySpace
{
	using MySpace::SceneManager::CScene;
	
	namespace Game
	{
		//--- クラス定義
		class CGameObject : public CBaseObject<CGameObject>
		{
			friend CComponent;
		private:
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_pLayer), CEREAL_NVP(m_pTag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_pTransform)*/
				);
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_pLayer), CEREAL_NVP(m_pTag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_pTransform)*/
				);
				
				// ｺﾝﾎﾟｰﾈﾝﾄ側でserial化するとエラー
				for (auto & com : m_pComponent)
				{
					com->SetOwner(GetPtr().lock());
				}
			}
		public:
			//--- 列挙体定義
			enum E_ObjectState // *@オブジェクトの状態(本来bool?)
			{
				ACTIVE,				// *@更新状態
				WAIT,				// *@待機(描画はされる)
				DESTROY,			// *@削除
				TAKEOVER,			// *@引き継ぎ（使いまわし)
				STOP,				// *@デバッグ?
				MAX_OBJECT_STATE,	// *@最大数
			};
			//--- エイリアス
			using Ptr = std::shared_ptr<CGameObject>;
			using PtrWeak = std::weak_ptr<CGameObject>;
		private:
			using COMPONENT = std::list<std::shared_ptr<CComponent>>;

		private:
			//--- メンバ変数
			COMPONENT m_pComponent;					// 所持コンポーネント
			E_ObjectState m_eState;					// 状態
			std::shared_ptr<CTag> m_pTag;			// タグ
			std::shared_ptr<CLayer> m_pLayer;		// レイヤー
			std::weak_ptr<CTransform> m_pTransform;	// パラメータ
			std::weak_ptr<CScene> m_pScene;			// 所属シーン

		public:
			//--- メンバ関数
			CGameObject();							// *@コンストラクタ
			CGameObject(std::string name);			// *@引き数月コンストラクタ
			CGameObject(const CGameObject& obj);	// *@コピーコンストラクタ
			virtual ~CGameObject();					// *@デストラクタ

			virtual void Awake();					// *@生成時呼び出される(実質Create)
			virtual void Init();					// *@初期化
			virtual void Update();					// *@ゲームオブジェクトごと更新
			virtual void LateUpdate();				// *@ゲームオブジェクトごと最終更新
			virtual void FixedUpdate();				// *@一定時間更新

			virtual void OnLoad();					// *@ﾃﾞｰﾀ読みこみ時呼び出し

			//--- 演算子のオーバーロード(使ってないかも 
			bool operator<(CGameObject* other) { return other->GetLayer() < this->GetLayer(); }
			bool operator<(std::shared_ptr<CGameObject> other) { return other->GetLayer() < this->GetLayer(); }
			bool operator>(CGameObject* other) { return other->GetLayer() > this->GetLayer(); }
			bool operator>(std::shared_ptr<CGameObject> other) { return other->GetLayer() > this->GetLayer(); }
			
			//--- コンポーネント関連

			// *@生成することで追加する
			// *@持ち主設定、自SPの設定などを行い、生成時処理を呼び出す
			template <class T = CComponent>
			std::shared_ptr<T> AddComponent()
			{
				std::shared_ptr<T> com = std::make_shared<T>(GetPtr().lock());
				com->SetPtr(com);					// 自身のポインタの設定
				com->SetOwner(GetPtr().lock());		// 所持gameobjectの設定

				com->Awake();
				AddComponent(com);				// 配列への追加
				return com;
			}
			
			// *@コンポーネントの追加
			// *@他onjのcomの場合、comの共有となる
			std::shared_ptr<CComponent> AddComponent(std::shared_ptr<CComponent> com);
			// *@コンポーネントの破棄
			template <class T>
			bool RemoveComponent(std::string comName);
			// *@コンポーネントの破棄(引き数:SP)
			bool RemoveComponent(std::weak_ptr<CComponent> com);
			// *@コンポーネントListの取得
			_NODISCARD inline COMPONENT GetComponentList() { return m_pComponent; }

			// *@コンポーネントの取得
			// *@weak_ptrが返る
			template <class T>
			_NODISCARD std::weak_ptr<T> GetComponent()
			{
				std::weak_ptr<T> component;
				for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it) 
				{
					component = std::dynamic_pointer_cast<T>((*it));
					// 監視対象の判定
					// lockはポインタ取得。expired は判定用?
					if (component.expired())continue;
					return component;
				}
				//return component;
				return std::weak_ptr<T>();	// 実質null
			}

			//--- ゲッター・セッター
			// *@描画状態の確認
			inline bool IsVision() { return (m_eState == E_ObjectState::ACTIVE || m_eState == E_ObjectState::WAIT); }
			// *@ｱｸﾃｨﾌﾞ状態の確認
			inline bool IsActive() { return (m_eState == E_ObjectState::ACTIVE); }
			// *@所属シーンの取得
			inline CScene* GetScene() { return m_pScene.lock().get(); }
			// *@ﾄﾗﾝｽﾌｫｰﾑの取得
			// *@引き数:int(sp取得)
			inline CTransform* GetTransform() { return m_pTransform.lock().get(); }
			// *@ﾄﾗﾝｽﾌｫｰﾑの取得
			inline virtual std::shared_ptr<CTransform> GetTransform(int n) { return m_pTransform.lock(); };
			// *@obj状態取得
			inline E_ObjectState GetState() { return m_eState; };															
			// *@ﾀｸﾞｸﾗｽの取得
			inline std::shared_ptr<CTag> GetTagPtr() { return m_pTag; };													
			// *@ﾀｸﾞ文字の取得
			inline std::string GetTag() { return GetTagPtr()->GetTag(); };													
			// *@ﾚｲﾔｸﾗｽの取得
			inline std::shared_ptr<CLayer> GetLayerPtr() { return m_pLayer; };												
			// *@ﾚｲﾔｰ番号
			inline int GetLayer() { return *m_pLayer->GetLayer(); };														

			// *@シーンの設定
			// *@TODO:呼び出せる場所を制限するべき
			inline void SetScene(std::weak_ptr<CScene> scene) { m_pScene = scene; };
			// *@レイヤー番号の変更
			inline void SetLayer(int layer) { m_pLayer->SetLayer(layer); };
			// *@レイヤーｸﾗｽの設定
			inline void SetLayer(CLayer::E_Layer layer) { m_pLayer->SetLayer(layer); };
			// *@タグ名の設定
			void SetTag(const std::string tag);
			// *@状態により親子関係にあるオブジェクトの状態を変える
			void SetState(const E_ObjectState state);
			//inline void SetTransform(std::shared_ptr<CTransform> trans) { m_pTransform = trans; };

			//--- コンポーネントが持つ衝突関数の呼び出し
			// *@当たり判定呼び出し
			// *@衝突時判定
			void OnCollisionEnter(CGameObject* obj);			
			// *@当たり判定呼び出し
			// *@衝突状態判定
			void OnCollisionStay(CGameObject* obj);				
			// *@当たり判定呼び出し
			// *@衝突終了時判定
			void OnCollisionExit(CGameObject* obj);				

			// *@当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			// *@衝突時判定
			void OnTriggerStay(CGameObject* obj);				
			// *@当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			// *@衝突状態判定
			void OnTriggerEnter(CGameObject* obj);				
			// *@当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			// *@衝突終了時判定
			void OnTriggerExit(CGameObject* obj);				

			//--- 静的メンバ関数
			// ゲームオブジェクト関係ショートカット用関数?(あまりいい方法ではない気がする)			
			
			// *@名前検索
			static std::weak_ptr<CGameObject> FindGameObject(std::string name);						
			// *@タグ名検索
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(std::string tag);				
			// *@タグ名検索
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(std::string tag);	
			// *@タグｸﾗｽ検索
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(CTag tag);						
			// *@タグ検索リスト取得
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(CTag tag);			
			// *@オブジェクトの生成
			static std::weak_ptr<CGameObject> CreateObject(CGameObject* pObj = nullptr);			
			// *@オブジェクトの生成(Debug版)
			static std::shared_ptr<CGameObject> CreateDebugObject(std::shared_ptr<CGameObject>);	
			// *@オブジェクト破棄
			static void Destroy(std::weak_ptr<CGameObject>);										
			// *@オブジェクト破棄除外登録
			static void DontDestroy(std::weak_ptr<CGameObject> obj);								

#ifdef BUILD_MODE
		private:
			int m_nCopyCnt = 0;
		public:
			virtual void ImGuiDebug();
#endif // BUILD_MODE
		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CGameObject)

#endif //__GAME_OBJECT_H__
