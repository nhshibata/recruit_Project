//=========================================================
// �쐬:2022/04/19
// �X�V:2022/05/11 �e�q�֌W�̎����B���[�J�����W�p�̸׽�쐬
// �g�����X�t�H�[�� : ���i�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <vector>

namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		//--- �O���Q��
		class CGameObject;
	}
}

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CTransform : public CComponent
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("transform", cereal::base_class<CComponent>(this)),
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/CEREAL_NVP(m_Rot)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("transform", cereal::base_class<CComponent>(this)),
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/CEREAL_NVP(m_Rot)
				);
			}
		private:
			//--- �����o�ϐ�
			Vector3 m_vPos;			// ���W
			Vector3 m_vOldPos;		// �ߋ����W
			Vector3 m_vRot;			// �p�x
			Vector3 m_vDestRot;		// �ڕW�p�x
			Vector3 m_vScale;		// �傫��
			Matrix4x4 m_mWorldMtx;
			Matrix4x4 m_mLocalMtx;
			Quaternion m_Rot;

		protected:
			std::weak_ptr<CGameObject> m_pParent;				// �e�I�u�W�F�N�g
			std::vector<std::weak_ptr<CGameObject>> m_pChilds;	// �q�v�f���X�g

		public:
			//--- �����o�֐�
			CTransform();
			// *@ �e�I�u�W�F�N�g��������
			CTransform(std::shared_ptr<CGameObject> owner);
			CTransform(std::shared_ptr<CGameObject> owner, Vector3 pos, Vector3 rot, Vector3 size);
			~CTransform();

			virtual void Uninit();
			void Init();
			void Update();

			// �Q�b�^�[�E�Z�b�^�[
			inline Vector3 GetPos() { if (!m_pParent.lock()) { return m_vPos; } return m_vPos; };
			inline Vector3 GetOldPos() { return m_vOldPos; };
			inline Vector3 GetRot() { if (!m_pParent.lock()) { return m_vRot; }return m_vPos; }
			inline Vector3 GetScale() { if (!m_pParent.lock()) { return m_vScale; } return m_vPos; };
			
			Quaternion GetWorldQuaternion();
			Quaternion GetLocalQuaternion();
			inline XMFLOAT4X4 GetWorldMatrix() { return m_mWorldMtx; }
			inline XMFLOAT4X4 GetLocalMatrix() { return m_mLocalMtx; }

			inline void SetPos(Vector3 value) { m_vPos = value; };
			inline void SetRot(Vector3 value) { m_vRot = value; };
			inline void SetDestRot(Vector3 value) { m_vDestRot = value; };
			inline void SetScale(Vector3 value) { m_vScale = value; };
			inline void SetWorldQuaternion(const Quaternion &  rotation);
			inline void SetLocalQuaternion(const Quaternion &  rotation);
			inline void SetWorldMatrix();
			inline void SetWorldMatrix(const XMFLOAT4X4 value) { m_mWorldMtx = value; }
			inline void SetLocalMatrix(const XMFLOAT4X4 value) { m_mLocalMtx = value; }

			Vector3 forward()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(m_vRot.x), DirectX::XMConvertToRadians(m_vRot.y), DirectX::XMConvertToRadians(m_vRot.z)));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			Vector3 right()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(m_vRot.y + 90.0f), DirectX::XMConvertToRadians(m_vRot.x),DirectX::XMConvertToRadians(m_vRot.z)));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			Vector3 up()
			{
				return Vector3::Normalize(Vector3::Cross(forward(), right()));
			}

			// *@ �e�I�u�W�F�N�g�擾
			inline std::weak_ptr<CGameObject> GetParent() { return m_pParent; };				// �e��Ԃ�
			// *@ �q�I�u�W�F�N�g�擾
			inline std::vector<std::weak_ptr<CGameObject>> GetChilds() { return m_pChilds; }
			// *@ �q�I�u�W�F�N�g�擾
			std::weak_ptr<CGameObject> GetChild(int no);
			inline int GetChildCount() { return (int)m_pChilds.size(); };	// �v�f����Ԃ�

			// *@ �e�v�f�ǉ�
			inline void SetParent(std::weak_ptr<CGameObject> parent) { m_pParent = parent; }
			// *@ �q�v�f�ǉ�
			void AddChild(std::weak_ptr<CGameObject> child) 
			{ 
				m_pChilds.push_back(child); 
				child.lock()->GetTransform()->SetParent(GetOwner(0));
			}
			// *@ �q�v�f���O
			void RemoveChild(std::weak_ptr<CGameObject> child) 
			{
				for (auto it = m_pChilds.begin(); it != m_pChilds.end(); ++it)
				{
					if ((*it).lock() == child.lock()) {
						m_pChilds.erase(it); break;
					}
				}
			}
#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CTransform)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CTransform)

#endif // !__TRANSFORM_H__

