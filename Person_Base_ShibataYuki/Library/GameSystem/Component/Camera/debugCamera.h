//=========================================================
// [debugCamera.h]
// �쐬:2022/07/19
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DEBUG_CAMERA_COMPONENT_H__
#define __DEBUG_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CDebugCamera : public CCamera
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("debugCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("debugCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
		private:
			enum class ECameraMode	// ��ב�����
			{
				CAM_MODE_NONE,		// ����Ȃ�
				CAM_MODE_ORBIT,		// �����_�𒆐S�ɉ�]
				CAM_MODE_TRACK,		// �����_�ƈꏏ�Ɉړ�
				CAM_MODE_DOLLY,		// �����_�ɋ߂Â����艓����������
			};

			ECameraMode m_eMode = ECameraMode::CAM_MODE_NONE;		// �J�������_
			bool m_bMouse;
			POINT m_oldMousePos;
		public:
			//--- ���ފ֐�
			CDebugCamera();
			CDebugCamera(std::shared_ptr<CGameObject> owner);
			~CDebugCamera();

			void Awake();
			void Init();					// ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			void Update();					// �X�V
			//void LateUpdate();			// �x���X�V

			void CameraMouseMove(int x, int y);
#ifdef BUILD_MODE

			//virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CDebugCamera)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CCamera, MySpace::CGameCamera)

#endif // !__GAME_CAMERA_H__
