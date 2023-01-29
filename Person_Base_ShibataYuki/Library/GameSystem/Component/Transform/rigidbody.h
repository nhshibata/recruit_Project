//=========================================================
// [rigidbody.cpp]
//---------------------------------------------------------
// �쐬:2022/05/24
//---------------------------------------------------------
// ����
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __RIGIDBODY_COMPONENT_H__
#define __RIGIDBODY_COMPONENT_H__

//---- �C���N���[�h��
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CRigidbody : public CComponent
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("RigidbodyComponent", cereal::base_class<CBaseComponent>(this))/*,
					CEREAL_NVP(m_bGravity), CEREAL_NVP(m_bIsSleep), CEREAL_NVP(m_bCollisionDetectionMode),
					CEREAL_NVP(m_fResistance)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("RigidbodyComponent", cereal::base_class<CBaseComponent>(this))/*,
					CEREAL_NVP(m_bGravity), CEREAL_NVP(m_bIsSleep), CEREAL_NVP(m_bCollisionDetectionMode),
					CEREAL_NVP(m_fResistance)*/
				);
			}
		private:
			//--- ���Œ�\����
			struct FixedVector3
			{
				// �V���A���C�Y
				friend class cereal::access;
				template<class Archive>
				void serialize(Archive & archive)
				{
					archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z),
							CEREAL_NVP(bX), CEREAL_NVP(bY), CEREAL_NVP(bZ)
					);
				}
				//--- �����o�ϐ�
				float x, y, z;
				bool bX, bY, bZ;

				//--- �����o�֐�
				FixedVector3()
				{
					x = y = z = 0;
					bX = bY = bZ = false;
				};
				void Fixed(bool pX, bool pY, bool pZ, Vector3 vec)
				{
					if (pX) { bX = true; x = vec.x; }
					if (pY) { bY = true; y = vec.y; }
					if (pZ) { bZ = true; z = vec.z; }
				}
				void Fixed(Vector3 vec)
				{
					if (bX) { x = vec.x; }
					if (bX) { y = vec.y; }
					if (bX) { z = vec.z; }
				}
				void Fix(Vector3 vec)
				{
					if (bX) vec.x = x; 
					if (bY) vec.y = y; 
					if (bZ) vec.z = z; 
				}
			};

		private:
			//--- ���ޕϐ�
			bool m_bGravity;			// �d�̓t���O
			bool m_bIsSleep;			//
			//bool m_bCollisionDetectionMode;		// �����蔻����s����
			float m_fResistance;		// ��R
			float m_fGravity;			// �d��
			float m_fMass;				// ����

			Vector3 m_vTargetPos;		// �ڕW���W
			Vector3 m_vVel;				// �ړ���
			Vector3 m_vForce;			// ��
			Vector3 m_vAccel;			// ����
			FixedVector3 m_pFreezPos;	// ���W�Œ�
			FixedVector3 m_pFreezRot;	// ��]�Œ�

		public:
			//--- �����o�֐�
			CRigidbody();
			CRigidbody(std::shared_ptr<CGameObject> owner);
			~CRigidbody();
			
			// *@�������Ăяo��
			virtual void Awake() {};
			void FixedUpdate();

			void OnCollisionEnter(CGameObject * obj);

			//--- �Q�b�^�[�E�Z�b�^�[
			_NODISCARD inline Vector3 GetVel() { return m_vVel; };
			_NODISCARD inline Vector3 GetAccel() { return m_vAccel; };

			inline void SetGravity(const bool value) { m_bGravity = value; }
			inline void SetTargetPos(Vector3 value) { m_vTargetPos = value; };
			inline void SetVel(Vector3 value) { m_vVel = value; };
			inline void SetAccel(Vector3 value) { m_vAccel = value; };
			// �ʒu�E�p�x�Œ�
			void SetFreezPos(const bool x, const bool y, const bool z);
			void SetFreezRot(const bool x, const bool y, const bool z);
			inline void SetForce(Vector3 value) { m_vForce = value; }

			_NODISCARD inline bool UseGravity() { return m_bGravity; }
			_NODISCARD inline bool IsSleeping() { return m_bIsSleep; }
			inline void AddForce(Vector3 add) { m_vForce += add; }

#ifdef BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
//CEREAL_REGISTER_TYPE(MySpace::Game::CRigidbody)

#endif // !__RIGIDBODY_H__
