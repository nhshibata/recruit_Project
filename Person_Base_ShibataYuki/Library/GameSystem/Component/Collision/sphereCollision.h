//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================
//--- インクルードガード
#ifndef __SPHERE_COLLISION_COMPONENT_H__
#define __SPHERE_COLLISION_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Collision/collision.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CSphereCollision : public CCollision
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("sphereCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_fRadius));
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("sphereCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_fRadius));
			}

		private:
			float m_fRadius;			// 当たり判定球
		public:
			//--- ﾒﾝﾊﾞ関数
			CSphereCollision() {};
			CSphereCollision(std::shared_ptr<CGameObject> owner, float radius = 1.0f);
			~CSphereCollision();

			//--- ゲッター・セッター
			inline float GetRadius() { return m_fRadius; };
			inline void SetRadius(float radius) { m_fRadius = radius; };

			bool Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br);
			bool CollisionSphere(Vector3 pos, float radius);

			//
			Vector3 PosAdjustment(Vector3 pos, Vector3 size) {};
			Vector3 PosAdjustment(Vector3 pos, float size);
			bool HitCheckPtr(CCollision* col);		// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
			
			void PushObject(CSphereCollision* other)
			{
				//---  押し出し
				// 二点間と２半径の差
				Vector3 distance = Transform()->GetPos() - other->Transform()->GetPos();
				float len = (GetRadius() + other->GetRadius()) - distance.Length();
				// 押し出す方向
				Vector3 vec = Vector3::Normalize(distance) * len;
				// 押し出し
				Transform()->SetPos(Transform()->GetPos() + vec);
			}

			virtual void ImGuiDebug();
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSphereCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CSphereCollision)

#endif // !__SPHERE_COLLISION_H__
