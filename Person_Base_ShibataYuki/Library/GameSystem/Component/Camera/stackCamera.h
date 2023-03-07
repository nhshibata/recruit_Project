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

			// *@�ݒ肳�ꂽ��ׂ̎擾
			std::vector<std::weak_ptr<CStackCamera>> GetStackCamera() { return m_aStackCamera; }
			
			// *@���[�h�擾
			bool IsStackMode(const EStackMode mode) { return m_eMode == mode; }

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

#endif // !__STACK_CAMERA_COMPONENT_H__