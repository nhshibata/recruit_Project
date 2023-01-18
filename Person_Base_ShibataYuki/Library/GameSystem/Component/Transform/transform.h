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
#include <GameSystem/GameObject/gameObject.h>
#include <vector>

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		class CGameObject;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CGameObject;

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
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/CEREAL_NVP(m_Rot)
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("transform", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/ CEREAL_NVP(m_Rot)
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
				);
			}
		private:
			//--- �����o�ϐ�
			Vector3 m_vPos;			// ���W
			Vector3 m_vOldPos;		// �ߋ����W
			Vector3 m_vRot;			// �p�x
			Vector3 m_vDestRot;		// �ڕW�p�x
			Vector3 m_vScale;		// �傫��
			Matrix4x4 m_mWorldMtx;	// ���[���h�}�g���b�N�X
			Matrix4x4 m_mLocalMtx;	// ���[�J���}�g���b�N�X
			Quaternion m_Rot;		// �N�H�[�^�j�I��

		protected:
			std::weak_ptr<CTransform> m_pParent;					// �e�I�u�W�F�N�g
			std::vector<std::weak_ptr<CTransform>> m_pChilds;		// �q�v�f���X�g
		
		private:
			//--- �����o�֐�
			void UpdateChildMatrix(CTransform* ptr, Matrix4x4 mtx);

		public:
			CTransform();
			CTransform(std::shared_ptr<CGameObject> owner);
			~CTransform();

			// *@�������Ăяo��
			virtual void Awake() {};
			void Init();
			virtual void Uninit();
			void Update();
			void UpdateWorldMatrix();

			//--- �Q�b�^�[�E�Z�b�^�[
			_NODISCARD inline Vector3 GetPos()		{ return m_vPos; };
			_NODISCARD inline Vector3 GetRot()		{ return m_vRot; };
			_NODISCARD inline Vector3 GetScale()	{ return m_vScale; };
			_NODISCARD inline Vector3 GetOldPos()	{ return m_vOldPos; };
			
			Quaternion GetWorldQuaternion();
			Quaternion GetLocalQuaternion();
			_NODISCARD inline XMFLOAT4X4 GetWorldMatrix() { return m_mWorldMtx; }
			_NODISCARD inline XMFLOAT4X4 GetLocalMatrix() { return m_mLocalMtx; }

			inline void SetPos(Vector3 value)					{ m_vPos = value; };
			inline void SetRot(Vector3 value)					{ m_vRot = value; };
			inline void SetDestRot(Vector3 value)				{ m_vDestRot = value; };
			inline void SetScale(Vector3 value)					{ m_vScale = value; };
			inline void SetWorldQuaternion(const Quaternion & rotation);
			inline void SetLocalQuaternion(const Quaternion & rotation);
			void SetWorldMatrix(Vector3 translate, Vector3 rot, Vector3 scale);
			inline void SetWorldMatrix(const XMFLOAT4X4 value)	{ m_mWorldMtx = value; }
			inline void SetLocalMatrix(const XMFLOAT4X4 value)	{ m_mLocalMtx = value; }

			//--- �e�q�֌W
			// *@�e�I�u�W�F�N�g�擾
			_NODISCARD inline std::weak_ptr<CTransform> GetParent()		{ return m_pParent; };
			// *@�q�I�u�W�F�N�g�擾
			_NODISCARD inline 
				std::vector<std::weak_ptr<CTransform>> GetChilds()		{ return m_pChilds; }
			// *@�q�I�u�W�F�N�g�擾
			_NODISCARD std::weak_ptr<CTransform> GetChild(int no);
			// *@�v�f����Ԃ�
			_NODISCARD inline int GetChildCount()						{ return static_cast<int>(m_pChilds.size()); };

			// *@�e�v�f�ǉ�
			inline void SetParent(std::weak_ptr<CTransform> parent)		{ m_pParent = parent; }
			// *@�q�v�f�ǉ�
			void AddChild(std::weak_ptr<CTransform> child);
			// *@�q�v�f���O
			void RemoveChild(std::weak_ptr<CTransform> child)
			{
				for (auto it = m_pChilds.begin(); it != m_pChilds.end(); ++it)
				{
					if ((*it).lock() == child.lock())
					{
						m_pChilds.erase(it); 
						break;
					}
				}
			}
			// *@�e�q�֌W����
			void ParentDissolved()
			{
				if (!m_pParent.lock())
					return;
				m_pParent.lock()->RemoveChild(BaseToDerived<CTransform>());
				m_pParent.reset();
			}

			//--- �I�u�W�F�N�g�����擾
			// *@�O��
			inline Vector3 forward()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(
						DirectX::XMConvertToRadians(m_vRot.x),
						DirectX::XMConvertToRadians(m_vRot.y),
						DirectX::XMConvertToRadians(m_vRot.z)
					));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			// *@�E��
			inline Vector3 right()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(
						DirectX::XMConvertToRadians(m_vRot.y + 90.0f),
						DirectX::XMConvertToRadians(m_vRot.x),
						DirectX::XMConvertToRadians(m_vRot.z)
					));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			// *@ ���
			inline Vector3 up()
			{
				return Vector3::Normalize(Vector3::Cross(forward(), right()));
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

