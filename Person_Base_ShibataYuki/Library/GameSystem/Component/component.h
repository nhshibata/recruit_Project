//=========================================================
//作成:2022/04/19 (火曜日)
// コンポーネントｸﾗｽ : 基底クラス
//=========================================================

//--- インクルードガード
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

//--- インクルード部
#define NOMINMAX

#include <GameSystem/Component/BaseComponent.h>
#include <GameSystem/GameObject/gameObject.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/Math/MyMath.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <memory>

//---
#define LIVE_POINTER			0

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		//--- 前方参照
		class CGameObject;
		class CTransform;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CComponent : public CBaseComponent
		{
		protected:
			friend CGameObject;
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_spThisPtr), /*CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_spThisPtr),/* CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
		public:
			//--- 再定義
			using Ptr = std::weak_ptr<CComponent>;
		private:
			//--- メンバ変数
			// *@自身のスマートポインタ
			Ptr m_spThisPtr;
			// *@親オブジェクト
			std::weak_ptr<CGameObject> m_pOwner;
			// *@アクティブ状態
			bool m_bActive;
#if BUILD_MODE
			std::string m_Name;
#endif // BUILD_MODE


		public:
			//--- ﾒﾝﾊﾞ関数
			CComponent();
			CComponent(std::shared_ptr<CGameObject> owner);
			virtual ~CComponent() {};

			//--- 基本
			// *@ 生成時に呼び出される
			virtual void Awake() {};
			// *@ 初期化 他コンポーネントの取得などを行う
			virtual void Init() {};				
			// *@ 更新 必ず実装
			virtual void Update() {};			
			// *@ 遅い更新
			virtual void LateUpdate() {};		
			// *@ 一定時間の更新
			virtual void FixedUpdate() {};		
			// *@ 読み込み時呼び出し関数
			virtual void OnLoad();

			//--- 仮想関数
			// 衝突したときに呼び出される関数
			// 実装したい時にオーバーライドする

			// *@ 接し続けているオブジェクトが渡される
			virtual void OnCollisionStay(CGameObject* obj) {};
			// *@ 接し始めたオブジェクトが渡される
			virtual void OnCollisionEnter(CGameObject* obj) {};
			// *@ 離れたオブジェクトが渡される
			virtual void OnCollisionExit(CGameObject* obj) {};
			// *@ 接し続けているオブジェクトが渡される(トリガー)
			virtual void OnTriggerStay(CGameObject* obj) {};
			// *@ 接し始めたオブジェクトが渡される(トリガー)
			virtual void OnTriggerEnter(CGameObject* obj) {};
			// *@ 離れたオブジェクトが渡される(トリガー)
			virtual void OnTriggerExit(CGameObject* obj) {};

			//--- CGameObject
			// *@gameobjectｸﾗｽからのAddComponent呼び出し
			// *@引き数ありで生ポインタ取得
			template <class T>
			inline std::shared_ptr<T> AddComponent()
			{
				return m_pOwner.lock()->AddComponent<T>();
			}

			// *@gameobjectｸﾗｽからのAddComponent呼び出し
			template <class T>
			inline T* AddComponent(int n)
			{
				return m_pOwner.lock()->AddComponent<T>().get();
			}

			// *@gameobjectｸﾗｽへ直接ｺﾝﾎﾟｰﾈﾝﾄを追加する 使わない?
			inline void AddComponent(std::shared_ptr<CComponent> com)
			{
				m_pOwner.lock()->AddComponent(com);
			}

			// *@持ち主から型指定したコンポーネントを取得する
			template <class T>
			inline T* GetComponent()
			{
				std::weak_ptr<T> com = m_pOwner.lock()->GetComponent<T>();
				return com.lock().get();
			};

			//--- セッター・ゲッター
			// *@トランスフォームポインタ取得
			CTransform* Transform()const;

			// *@ tag取得
			// *@ ownerがnullならエラー
			std::string Tag()const;
			
			// *@レイヤー番号取得
			int GetLayer()const;

			// *@自身のSPを派生クラスへキャスト
			template <class T>
			inline std::shared_ptr<T> BaseToDerived() { return std::dynamic_pointer_cast<T>(m_spThisPtr.lock()); }

			// *@型名の取得
			inline std::string GetName() { return typeid(*this).name(); }

			// *@weakPtrの取得			
			inline Ptr GetPtr() { return m_spThisPtr; }

			// *@Componentの持ち主GameObject型の取得
			// *@引き数なしで生ポインタを受け取る
			// *@引き数ありでweak_ptrを受け取る
			inline CGameObject* GetOwner() { return m_pOwner.lock().get(); }

			// *@Componentの持ち主GameObject型の取得
			// *@引き数なしで生ポインタを受け取る
			// *@引き数ありでweak_ptrを受け取る
			inline std::weak_ptr<CGameObject> GetOwner(int no) { return m_pOwner.lock(); };

			// *@ｺﾝﾎﾟｰﾈﾝﾄのｱｸﾃｨﾌﾞ設定
			inline void SetActive(bool flg) { m_bActive = flg; };

			// *@自身のSPを引き渡す
			// NOTE: オブジェクト、コンポーネント生成時にポインタを渡さなければいけない
			inline void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

			// *@持ち主を設定
			inline void SetOwner(std::weak_ptr<CGameObject> pOwner) { m_pOwner = pOwner; }

			// *@ｱｸﾃｨﾌﾞ状態取得
			// *@持ち主もActiveでなければfalse
			bool IsActive();
		
		};

	}

}
using namespace MySpace::Game;
using namespace MySpace::MyMath;

CEREAL_REGISTER_TYPE(MySpace::Game::CComponent)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, CBaseComponent<CComponent>)

#endif __COMPONENT_H__
