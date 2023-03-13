//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================
//--- インクルードガード
#ifndef __SPHERE_COLLISION_COMPONENT_H__
#define __SPHERE_COLLISION_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Collision/collision.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Game
	{
		class CBoxCollision;
	}
	namespace Graphics
	{
		class CSphere;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		using MySpace::Graphics::CSphere;

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
			//--- メンバ変数
			float m_fRadius;			// 当たり判定球

		private:
			//--- ﾒﾝﾊﾞ関数
			// *@押し出し
			void PushBack(CCollision* other, float radius);
			bool BoxSphere(CBoxCollision* box, Game::CSphereCollision* sphere);

		public:
			CSphereCollision();
			CSphereCollision(std::shared_ptr<CGameObject> owner, float radius = 1.0f);
			~CSphereCollision();

			//--- 当たり判定
			bool Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br);
			bool CollisionSphere(Vector3 pos, float radius);

			// *@コリジョンｸﾗｽを引き数にとって、当たり判定を行う
			bool HitCheckPtr(CCollision* col);

			//--- ゲッター・セッター
			inline float GetRadius() { return m_fRadius; };
			inline void SetRadius(float radius) { m_fRadius = radius; };

#ifdef BUILD_MODE
		private:
			std::shared_ptr<CSphere> m_pDebugSphere;
		public:
			virtual void ImGuiDebug();
			virtual void Update();
#endif

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSphereCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CSphereCollision)

#endif // !__SPHERE_COLLISION_H__
