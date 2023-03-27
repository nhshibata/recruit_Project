//==========================================================
// [stackCamera.h]
//---------------------------------------------------------
// �쐬�F2023/02/14
//---------------------------------------------------------
//
//==========================================================

#ifndef __STACK_CAMERA_COMPONENT_H__
#define __STACK_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/layerCamera.h>
#include <GraphicsSystem/DirectX/GBuffer.h>

namespace MySpace
{
	namespace Game
	{
		class CStackCamera : public CLayerCamera
		{
			//--- �V���A���C�Y
#pragma region _cereal
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("stackCamera", cereal::base_class<CLayerCamera>(this)),
						CEREAL_NVP(m_eMode), CEREAL_NVP(m_aStackCamera)
				); 
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("stackCamera", cereal::base_class<CLayerCamera>(this)),
						CEREAL_NVP(m_eMode), CEREAL_NVP(m_aStackCamera)
				);
			}
#pragma endregion
		public:
			enum class EStackMode
			{
				BASE,		// stack�\��ԁBMainCamera�łȂ���ΈӖ����Ȃ�
				OVERLAY		// stack�Ώۏ�ԁBBase��stack�ł���
			};

		private:
			EStackMode m_eMode;
			std::vector<std::weak_ptr<CStackCamera>> m_aStackCamera;
			std::unique_ptr<MySpace::Graphics::CGBuffer> m_pGBuf;

		public:
			CStackCamera();
			CStackCamera(CGameObject::Ptr owner);
			~CStackCamera();

			// *@stack�ɒǉ�
			// *@�������͏��Ԏw��
			void Stack(std::weak_ptr<CStackCamera> camera, int idx = -1);
			
			// *@�}�X�N�Ώۂ��m�F
			bool IsMask(const int layer)override final;

			// *@���[�h�擾
			bool IsStackMode(const EStackMode mode) { return m_eMode == mode; }

			// *@layer�Ώ�
			const int GetAllTargetLayer();

			// *@�ݒ肳�ꂽ��ׂ̎擾
			std::vector<std::weak_ptr<CStackCamera>> GetStackCameras() { return m_aStackCamera; }

			// *@��������GBuffer
			MySpace::Graphics::CGBuffer* GetGBuffer()const { return m_pGBuf.get(); }
			
			// *@���[�h�擾
			const EStackMode GetStackMode() { return m_eMode; }

			// *@���[�h�ݒ�
			void SetStackMode(const EStackMode mode) { m_eMode = mode; }



#ifdef BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MDOE


		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CStackCamera)

#endif // !__STACK_CAMERA_COMPONENT_H__