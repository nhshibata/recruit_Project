//=========================================================
// 作成:2022/04/20 (水曜日)
// コリジョンｸﾗｽ(当たり判定 ) : 基底ｸﾗｽ
//=========================================================
//--- インクルードガード
#ifndef __COLLISION_COMPONENT_H__
#define __COLLISION_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CCollision : public CComponent
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("collision", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bIsTrigger)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("collision", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bIsTrigger)
				);
			}
		protected:
			//--- ﾒﾝﾊﾞ変数
			Vector3 m_vOldPos;			// 衝突があった場合、元に戻す座標
			Vector3 m_vOldScale;		// 衝突があった場合、元に戻す座標

			std::weak_ptr<CTransform> m_pTransform;	// ownerのTransformへのポインタ(関数で取得する無駄を省く)

			bool m_bIsTrigger;				// 当たった時の動作(trueですり抜け)

			std::list<std::weak_ptr<CGameObject>> m_pOldStayList;	// 1フレーム前に接触していた
			std::list<std::weak_ptr<CGameObject>> m_pHitList;		// 現在フレーム接触
			std::list<std::weak_ptr<CGameObject>> m_pExitList;		// 離れた

#ifdef BUILD_MODE
			int m_nDebugEnterCnt = 0;
			int m_nDebugStayCnt  = 0;
			int m_nDebugExitCnt  = 0;
#endif // BUILD_MODE

		private:
			//--- ﾒﾝﾊﾞ関数
			void ColObjectUpdate();
		protected:
			void HitResponse(CCollision* col);
			Vector3 PosAdjustment(Vector3 pos, Vector3 size) {};

		public:
			//--- ﾒﾝﾊﾞ関数
			CCollision() :m_bIsTrigger(false) {};
			CCollision(std::shared_ptr<CGameObject> owner, bool trigger = false);
			~CCollision();

			void Awake();
			void Init();
			void Update();
			void RequestCollision();

			//--- セッター・ゲッター
			//std::string GetColTag() { return m_HitSubjectTag; };
			// *@ 判定を行う際のすりぬけ確認
			inline bool IsTrigger() { return m_bIsTrigger; };

			/*void SetColTag(std::string tag) { m_HitSubjectTag = tag; };*/

			// *@ 判定を行う際のすりぬけ設定
			inline void SetTrigger(const bool is) { m_bIsTrigger = is; };

			// <Summary>
			// 当たり判定方法により異なる
			// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
			// </Summary>
			virtual bool HitCheckPtr(CCollision* col) { return false; };

			// <Summary> 
			// 離れたオブジェクトを持ち主に教える
			// </Summary>
			virtual bool ExitTell();

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CCollision)

#endif // !__COLLISION_H__

