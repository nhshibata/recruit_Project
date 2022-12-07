//=========================================================
//�쐬:2022/05/02
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __SPHERE_COLLISION_COMPONENT_H__
#define __SPHERE_COLLISION_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/collision.h>

#pragma region ForwardDeclaration
namespace MySpace
{
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
			CSphereCollision(std::shared_ptr<CGameObject> owner, float radius = 10.0f);
			~CSphereCollision();

			bool Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br);
			bool CollisionSphere(Vector3 pos, float radius);

			Vector3 PosAdjustment(Vector3 pos, Vector3 size) {};
			Vector3 PosAdjustment(Vector3 pos, float size);

			// *@�R���W�����׽���������ɂƂ��āA�����蔻����s��
			bool HitCheckPtr(CCollision* col);
			
			// *@�����o��
			void PushObject(CSphereCollision* other);

			//--- �Q�b�^�[�E�Z�b�^�[
			inline float GetRadius() { return m_fRadius; };
			inline void SetRadius(float radius) { m_fRadius = radius; };

#ifdef BUILD_MODE
		private:
			std::shared_ptr<CSphere> m_pDebugSphere;
		public:
			virtual void ImGuiDebug();
#endif

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CSphereCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CSphereCollision)

#endif // !__SPHERE_COLLISION_H__
