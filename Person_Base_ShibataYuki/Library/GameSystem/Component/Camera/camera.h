//=========================================================
// [camera.h]
// �쐬:2022/06/27
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Game
	{
		class CModelRenderer;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CCamera : public CComponent
		{
		private:
			// �V���A���C�Y
#pragma region _cereal
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("camera", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_vTarget), CEREAL_NVP(m_vUp), CEREAL_NVP(m_vAngle), CEREAL_NVP(m_fLengthInterval),
					CEREAL_NVP(m_fAspectRatio), CEREAL_NVP(m_fFovY), CEREAL_NVP(m_fNearZ), CEREAL_NVP(m_fFarZ),
					CEREAL_NVP(m_mtxWorld), CEREAL_NVP(m_mtxView), CEREAL_NVP(m_mtxProj), 
					CEREAL_NVP(m_pSky)/* CEREAL_NVP(m_pCamera)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("camera", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_vTarget), CEREAL_NVP(m_vUp), CEREAL_NVP(m_vAngle), CEREAL_NVP(m_fLengthInterval),
					CEREAL_NVP(m_fAspectRatio), CEREAL_NVP(m_fFovY), CEREAL_NVP(m_fNearZ), CEREAL_NVP(m_fFarZ),
					CEREAL_NVP(m_mtxWorld), CEREAL_NVP(m_mtxView), CEREAL_NVP(m_mtxProj), 
					CEREAL_NVP(m_pSky)/*CEREAL_NVP(m_pCamera)*/
				);
			}
#pragma endregion

		public:
			enum class EFrustumResult
			{
				INSIDE,
				OUTSIDE,
				PARTINSIDE,
				MAX
			};
			static const int MAX_FRUS = 6;	// ���o��

		protected:
			DirectX::XMFLOAT3 m_vPos;	// ���_
			Vector3 m_vTarget;			// �����_
			Vector3 m_vUp;				// ����x�N�g��
			Vector3 m_vAngle;			// �J�����̊p�x

			float m_fLengthInterval;	// �J�����̎��_�ƒ����_�̋���

		private:
			float m_fAspectRatio;		// �c����
			float m_fFovY;				// ����p(Degree)
			float m_fNearZ;				// �O���N���b�v����
			float m_fFarZ;				// ����N���b�v����

			Matrix4x4 m_mtxWorld;		// ���[���h �}�g���b�N�X
			Matrix4x4 m_mtxView;		// �r���[ �}�g���b�N�X
			Matrix4x4 m_mtxProj;		// �v���W�F�N�V���� �}�g���b�N�X

			std::weak_ptr<CModelRenderer> m_pSky;	// �X�J�C�h�[��ptr

			XMFLOAT4 m_frus[MAX_FRUS];
			XMFLOAT4 m_frusw[MAX_FRUS];

			static inline std::weak_ptr<CCamera> m_pMainCamera;	// ���C���J����

		private:
			//--- ���ފ֐�
			void InitFrustum();
			void UpdateFrustum();

		public:
			CCamera();
			CCamera(std::shared_ptr<CGameObject> owner);
			~CCamera();

			void OnLoad();
			void Awake();
			void Init();					// ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			void Update();					// �X�V
			//void LateUpdate();			// �x���X�V

			void DrawSkyDome();
			void UpdateMatrix();
			DirectX::XMFLOAT4X4& CalcWorldMatrix();

			inline void SetPos(DirectX::XMFLOAT3& vPos) { Transform()->SetPos(vPos); }
			inline void SetPos(float x, float y, float z) { Transform()->SetPos(Vector3(x, y, z)); }
			inline void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
			inline void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
			inline void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
			inline void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
			inline void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
			inline void SetFovY(float fFovY) { m_fFovY = fFovY; }
			inline void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
			inline void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }

			inline DirectX::XMFLOAT3& GetPos() { return m_vPos; }
			inline DirectX::XMFLOAT3 GetUpVector() { return m_vUp; }
			inline DirectX::XMFLOAT3 GetTarget() { return m_vTarget; }
			inline DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
			inline DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
			inline DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
			inline DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
			inline float GetAspect() { return m_fAspectRatio; }
			inline float GetFarZ() { return m_fFarZ; }
			inline float GetFOV() { return m_fFovY; }
			inline float GetNearClip() { return m_fNearZ; }

			inline DirectX::XMMATRIX GetWorldMatrix(int no) { return DirectX::XMLoadFloat4x4(&m_mtxWorld); }
			inline DirectX::XMMATRIX GetLookAtMatrix() { return DirectX::XMLoadFloat4x4(&m_mtxView); };
			inline DirectX::XMMATRIX GetProjectionMatrix() { return DirectX::XMLoadFloat4x4(&m_mtxProj); };
			
			// ������i�����蔻��)
			EFrustumResult CollisionViewFrustum(XMFLOAT3* pCenter, float fRadius);
			
			// �X�N���[�����W��3D���W�֕ϊ�
			inline Vector3 ConvertScreenToWorld(Vector2 pos)
			{
				Vector3 ret;
				D3D11_VIEWPORT& vp = *MySpace::Graphics::CDXDevice::Get().GetViewPort();
				XMStoreFloat3(&ret, XMVector3Unproject(
					XMVectorSet(pos.x, pos.y, 0.0f, 1.0f),
					vp.TopLeftX, 
					vp.TopLeftY, 
					vp.Width, 
					vp.Height, 
					vp.MinDepth, 
					vp.MaxDepth,
					XMLoadFloat4x4(&GetProjMatrix()), 
					XMLoadFloat4x4(&GetViewMatrix()),
					XMMatrixIdentity()
				));
				return ret;
			}

			// �Z�b�^�[�E�Q�b�^�[
			inline static CCamera* GetMain() { return m_pMainCamera.lock().get(); };
			inline static std::weak_ptr<CCamera> GetMain(int) { return m_pMainCamera.lock(); };

			inline void SetMain(std::weak_ptr<CCamera> ptr) { m_pMainCamera = ptr; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();
			
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CCamera)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CCamera)

#endif // !__CAMERA_H__
