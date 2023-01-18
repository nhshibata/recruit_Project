//=========================================================
// [component.h]
//---------------------------------------------------------
//作成:2022/04/19
//---------------------------------------------------------
// コンポーネントｸﾗｽ : 基底クラス
//=========================================================

//--- インクルードガード
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#define NOMINMAX

//--- インクルード部
#include <GameSystem/Component/BaseComponent.h>
#include <GameSystem/GameObject/gameObject.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/Math/MyMath.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		//--- 前方参照
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
					CEREAL_NVP(m_pSelfPtr), /*CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_pSelfPtr),/* CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
		public:
			//--- エイリアス
			using Ptr = std::weak_ptr<CComponent>;

		private:
			//--- メンバ変数
			Ptr m_pSelfPtr;							// 自身のスマートポインタ
			std::weak_ptr<CGameObject> m_pOwner;	// 親オブジェクト	
			bool m_bActive;							// アクティブ状態

		public:
			//--- ﾒﾝﾊﾞ関数
			CComponent();
			CComponent(const CComponent& copy);
			CComponent(std::shared_ptr<CGameObject> owner);
			virtual ~CComponent();

			//--- 基本
			// *@生成時に呼び出される
			virtual void Awake() = 0;

			// *@初期化 他コンポーネントの取得などを行う
			virtual void Init();

			// *@終了処理
			virtual void Uninit();
			// *@更新 必ず実装
			virtual void Update();
			// *@遅い更新
			virtual void LateUpdate();
			// *@一定時間の更新
			virtual void FixedUpdate();
			// *@読み込み時呼び出し関数
			virtual void OnLoad();

			//--- 当たり判定呼び出し
			// 衝突したときに呼び出される関数
			// 実装したい時にオーバーライドする

			// *@当たり判定
			// *@衝突時
			virtual void OnCollisionEnter(CGameObject* obj);
			// *@当たり判定
			// *@衝突状態
			virtual void OnCollisionStay(CGameObject* obj);
			// *@当たり判定
			// *@衝突終了時
			virtual void OnCollisionExit(CGameObject* obj);
			
			// *@当たり判定
			// *@衝突時
			virtual void OnTriggerEnter(CGameObject* obj);
			// *@当たり判定
			// *@衝突状態
			virtual void OnTriggerStay(CGameObject* obj);
			// *@当たり判定
			// *@衝突終了時
			virtual void OnTriggerExit(CGameObject* obj);

			//--- CGameObject
			// *@gameobjectｸﾗｽからのAddComponent呼び出し
			// *@引き数ありで生ポインタ取得
			template <class T>
			inline std::shared_ptr<T> AddComponent() { return m_pOwner.lock()->AddComponent<T>(); }

#pragma warning(push)
#pragma warning(disable:4100)
			// *@gameobjectｸﾗｽからのAddComponent呼び出し
			template <class T>
			inline T* AddComponent(int n) { return m_pOwner.lock()->AddComponent<T>().get(); }
#pragma warning(pop)

			// *@持ち主から型指定したコンポーネントを取得する
			template <class T>
			inline T* GetComponent() { std::weak_ptr<T> com = m_pOwner.lock()->GetComponent<T>(); return com.lock().get(); };

			//--- セッター・ゲッター
			// *@自身のSPを派生クラスへキャスト
			template <class T>
			inline std::shared_ptr<T> BaseToDerived() { return std::dynamic_pointer_cast<T>(m_pSelfPtr.lock()); }

			// *@トランスフォームポインタ取得
			CTransform* Transform()const;

			// *@tag取得
			// *@ownerがnullならエラー
			std::string Tag()const;
			
			// *@レイヤー番号取得
			int GetLayer()const;

			// *@型名の取得
			inline std::string GetName() { return typeid(*this).name(); }

			// *@weak_ptr(Sp)の取得			
			inline Ptr GetPtr() const { return m_pSelfPtr; }

			// *@Componentの持ち主GameObject型の取得
			// *@引き数なしで生ポインタを受け取る
			// *@引き数ありでweak_ptrを受け取る
			inline CGameObject* GetOwner() const { return m_pOwner.lock().get(); }

#pragma warning(push)
#pragma warning(disable:4100)
			// *@Componentの持ち主GameObject型の取得
			// *@引き数なしで生ポインタを受け取る
			// *@引き数ありでweak_ptrを受け取る
			inline std::weak_ptr<CGameObject> GetOwner(int no)const { return m_pOwner.lock(); };
#pragma warning(pop)

			// *@ｺﾝﾎﾟｰﾈﾝﾄのｱｸﾃｨﾌﾞ設定
			inline void SetActive(bool flg) { m_bActive = flg; };

			// *@自身のSPを引き渡す
			// NOTE: オブジェクト、コンポーネント生成時にポインタを渡さなければいけない
			inline void SetPtr(Ptr ptr) { m_pSelfPtr = ptr; }

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
