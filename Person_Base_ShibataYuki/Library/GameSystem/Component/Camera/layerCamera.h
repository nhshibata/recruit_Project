//==========================================================
// [layerCamera.h]
//---------------------------------------------------------
// �쐬:2023/02/14 
//---------------------------------------------------------
// ���C���[�}�X�N�������������
//==========================================================

#ifndef __LAYER_CAMERA_COMPONENT_H__
#define __LAYER_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{

		class CLayerCamera : public CCamera
		{
			//--- �V���A���C�Y
#pragma region _cereal
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("layerCamera", cereal::base_class<CCamera>(this)),
						CEREAL_NVP(m_nLayerMask)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("layerCamera", cereal::base_class<CCamera>(this)),
						CEREAL_NVP(m_nLayerMask)
				);
			}
#pragma endregion
		protected:
			int m_nLayerMask; // �`�悷��Layer�Ǘ��p

		public:
			CLayerCamera();
			CLayerCamera(std::shared_ptr<CGameObject> owner);
			~CLayerCamera();

			// *@�����}�X�N
			inline const int GetMask() const{ return m_nLayerMask; }
			// *@�}�X�N�Ώۂ��m�F
			virtual bool IsMask(const int layer)const override;
			// *@�}�X�N�ݒ�
			void SetMask(const int mask);
			// *@�}�X�N�ݒ�ǉ�
			void AddMask(const int laerNo);
			// *@�}�X�N�ݒ����
			void ReleaseMask(const int layerNo);

#ifdef BUILD_MODE
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CLayerCamera)

#endif // !__LAYER_CAMERA_COMPONENT_H__

