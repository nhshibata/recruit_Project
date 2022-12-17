//=========================================================
// [gameCamera.h]
// �쐬:2022/07/19
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_CAMERA_COMPONENT_H__
#define __GAME_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CGameCamera : public CCamera
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("gameCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("gameCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
		public:
			//--- ���ފ֐�
			CGameCamera();
			CGameCamera(std::shared_ptr<CGameObject> owner);
			~CGameCamera();

			void Awake();
			void Init();					// ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			void Update();					// �X�V
			//void LateUpdate();			// �x���X�V

			//virtual void ImGuiDebug();
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CGameCamera)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CCamera, MySpace::CGameCamera)

#endif // !__GAME_CAMERA_H__
