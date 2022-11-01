//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================
//--- インクルードガード
#ifndef __BOX_COLLISION_COMPONENT_H__
#define __BOX_COLLISION_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Collision/collision.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CBoxCollision : public CCollision
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(cereal::make_nvp("boxCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_vSize));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("boxCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_vSize));
			}
		private:
			Vector3 m_vSize;	// 当たり判定サイズ

		public:
			CBoxCollision() {};
			CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size = Vector3(1, 1, 1));
			~CBoxCollision();

			//--- ゲッター・セッター
			inline Vector3 GetSize() { return m_vSize; };
			inline void SetSize(Vector3 size) { m_vSize = size; };

			bool Box(Vector3 Apos, Vector3 Asize, Vector3 Bpos, Vector3 Bsize);
			bool CollisionAABB(Vector3 pos, Vector3 size);

			Vector3 PosAdjustment(Vector3 pos, Vector3 size);
			bool HitCheckPtr(CCollision* col);		// コリジョンｸﾗｽを引き数にとって、当たり判定を行う

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CBoxCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CBoxCollision)

#endif // __BOX_COLLISION_H__
