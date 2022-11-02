//=========================================================
//作成:2022/04/19 (火曜日)
// ゲームオブジェクト : 基底クラス
// 
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
					CEREAL_NVP(m_eState), CEREAL_NVP(m_Layer), CEREAL_NVP(m_Tag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_Transform)*/
				);
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_Layer), CEREAL_NVP(m_Tag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_Transform)*/
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
			//--- 再定義
			using Ptr = std::shared_ptr<CGameObject>;
			using PtrWeak = std::weak_ptr<CGameObject>;

		private:
			//--- 再定義
			using COMPONENT = std::list<std::shared_ptr<CComponent>>;

			//--- メンバ変数
			COMPONENT m_pComponent;					// 所持コンポーネント
			E_ObjectState m_eState;					// 状態
			std::shared_ptr<CTag> m_Tag;			// タグ
			std::shared_ptr<CLayer> m_Layer;		// レイヤー
			std::weak_ptr<CTransform> m_Transform;	// パラメータ
			std::weak_ptr<CScene> m_pScene;

		public:
			CGameObject();							// *@コンストラクタ
			CGameObject(std::string name);			// *@引き数月コンストラクタ
			CGameObject(const CGameObject& obj);	// *@コピーコンストラクタ
			virtual ~CGameObject();					// *@デストラクタ

			virtual void Awake();				// *生成時呼び出される(実質Create)
			virtual void Init();				// *初期化
			virtual void Update();				// *ゲームオブジェクトごとの更新
			virtual void LateUpdate();			// *ゲームオブジェクトごとの更新
			virtual void FixedUpdate();			// *一定時間の更新

			virtual void OnLoad();					// *@ﾃﾞｰﾀ読みこみ時呼び出し

			//--- 演算子のオーバーロード(使ってないかも 
			bool operator<(CGameObject* other) { return other->GetLayer() < this->GetLayer(); }
			bool operator<(std::shared_ptr<CGameObject> other) { return other->GetLayer() < this->GetLayer(); }
			bool operator>(CGameObject* other) { return other->GetLayer() > this->GetLayer(); }
			bool operator>(std::shared_ptr<CGameObject> other) { return other->GetLayer() > this->GetLayer(); }
			
			//--- コンポーネント
			// *生成することで追加する
			// *持ち主設定、自SPの設定などを行い、生成時処理を呼び出す
			template <class T>
			std::shared_ptr<T> AddComponent()
			{
				//static_assert(std::is_base_of<CComponent, T> == false, "not CComponent");// ｺﾝﾎﾟｰﾈﾝﾄを継承しているか確認

				std::shared_ptr<T> com = std::make_shared<T>(GetPtr().lock());
				com->SetPtr(com);					// 自身のポインタの設定
				com->SetOwner(GetPtr().lock());		// 所持gameobjectの設定

				com.get()->Awake();
				AddComponent(com);				// 配列への追加
				return com;
			}

			// *コンポーネントの追加
			// *他onjのcomの場合、comの共有となる
			std::shared_ptr<CComponent> AddComponent(std::shared_ptr<CComponent> com);
			// *コンポーネントの破棄
			template <class T>
			bool RemoveComponent(std::string comName);
			// *コンポーネントの破棄(引き数:SP)
			bool RemoveComponent(std::weak_ptr<CComponent> com);
			// *コンポーネントListの取得
			inline COMPONENT GetComponentList() { return m_pComponent; }

			// *コンポーネントの取得
			// *weak_ptrが返る
			template <class T>
			std::weak_ptr<T> GetComponent()
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

			// ゲッター・セッター

			inline bool IsVision() { return (m_eState == E_ObjectState::ACTIVE || m_eState == E_ObjectState::WAIT); }	// *描画状態の確認
			inline bool IsActive() { return (m_eState == E_ObjectState::ACTIVE); }										// *ｱｸﾃｨﾌﾞ状態の確認
			inline CScene* GetScene() { return m_pScene.lock().get(); }
			inline CTransform* GetTransform() { return m_Transform.lock().get(); };										// *ﾄﾗﾝｽﾌｫｰﾑの取得
			inline virtual std::shared_ptr<CTransform> GetTransform(int n) { return m_Transform.lock(); };
			inline E_ObjectState GetState() { return m_eState; };														// *obj状態取得
			inline std::shared_ptr<CTag> GetTagPtr() { return m_Tag; };													// *ﾀｸﾞｸﾗｽの取得
			inline std::string GetTag() { return GetTagPtr()->GetTag(); };												// *ﾀｸﾞ文字の取得
			inline std::shared_ptr<CLayer> GetLayerPtr() { return m_Layer; };											// *ﾚｲﾔｸﾗｽの取得
			inline int GetLayer() { return *m_Layer->GetLayer(); };														// *ﾚｲﾔｰ番号

			inline void SetScene(std::weak_ptr<CScene> scene) { m_pScene = scene; };
			inline void SetTransform(std::shared_ptr<CTransform> trans) { m_Transform = trans; };
			inline void SetState(const E_ObjectState state) { m_eState = state; }// TODO: 状態により親子関係にあるオブジェクトの状態を変える
			inline void SetLayer(int layer) { m_Layer->SetLayer(layer); };
			inline void SetLayer(CLayer::E_Layer layer) { m_Layer->SetLayer(layer); };
			void SetTag(const std::string tag);

			//--- 仮想関数
			// コンポーネントが持つ衝突関数の呼び出し
			void OnCollisionEnter(CGameObject* obj);			// *当たり判定呼び出し
			void OnCollisionStay(CGameObject* obj);			// *当たり判定呼び出し
			void OnCollisionExit(CGameObject* obj);			// *当たり判定呼び出し
			void OnTriggerStay(CGameObject* obj);				// *当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			void OnTriggerEnter(CGameObject* obj);			// *当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			void OnTriggerExit(CGameObject* obj);				// *当たり判定呼び出し(ﾄﾘｶﾞｰ時)
			//void HitCheck(CGameObject* obj);

			//--- 静的メンバ関数
			// ゲームオブジェクト関係ショートカット用関数?(あまりいい方法ではない気がする)
			static std::weak_ptr<CGameObject> FindGameObject(std::string name);						// *名前検索
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(std::string tag);				// *タグ名前検索
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(std::string tag);	// *タグ名前検索
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(CTag tag);						// *タグ検索
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(CTag tag);			// *タグ検索リスト
			static std::weak_ptr<CGameObject> CreateObject(CGameObject* pObj = nullptr);			// *オブジェクトの生成
			static void Destroy(std::weak_ptr<CGameObject>);										// *オブジェクト破棄
			static void DontDestroy(std::weak_ptr<CGameObject> obj);								// *オブジェクト破棄除外

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CGameObject)

#endif //__GAME_OBJECT_H__
