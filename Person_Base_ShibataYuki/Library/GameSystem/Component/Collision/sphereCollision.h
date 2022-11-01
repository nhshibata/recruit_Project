//=========================================================
//�쐬:2022/05/02
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __SPHERE_COLLISION_COMPONENT_H__
#define __SPHERE_COLLISION_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/collision.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CSphereCollision : public CCollision
		{
		private:
			// �V���A���C�Y
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
			float m_fRadius;			// �����蔻�苅
		public:
			//--- ���ފ֐�
			CSphereCollision() {};
			CSphereCollision(std::shared_ptr<CGameObject> owner, float radius = 1.0f);
			~CSphereCollision();

			//--- �Q�b�^�[�E�Z�b�^�[
			inline float GetRadius() { return m_fRadius; };
			inline void SetRadius(float radius) { m_fRadius = radius; };

			bool Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br);
			bool CollisionSphere(Vector3 pos, float radius);

			//
			Vector3 PosAdjustment(Vector3 pos, Vector3 size) {};
			Vector3 PosAdjustment(Vector3 pos, float size);
			bool HitCheckPtr(CCollision* col);		// �R���W�����׽���������ɂƂ��āA�����蔻����s��
			
			void PushObject(CSphereCollision* other)
			{
				//---  �����o��
				// ��_�ԂƂQ���a�̍�
				Vector3 distance = Transform()->GetPos() - other->Transform()->GetPos();
				float len = (GetRadius() + other->GetRadius()) - distance.Length();
				// �����o������
				Vector3 vec = Vector3::Normalize(distance) * len;
				// �����o��
				Transform()->SetPos(Transform()->GetPos() + vec);
			}

			virtual void ImGuiDebug();
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSphereCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CSphereCollision)

#endif // !__SPHERE_COLLISION_H__
