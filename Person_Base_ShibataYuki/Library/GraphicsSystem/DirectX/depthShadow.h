//=========================================================
// [depthShadow.h]
//---------------------------------------------------------
// �쐬:2023/01/23
//---------------------------------------------------------
//=========================================================

#ifndef __DEPTH_SHADOW_H__
#define __DEPTH_SHADOW_H__

//--- �C���N���[�h��
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>
#include <GraphicsSystem/Shader/constantBuffer.h>

namespace MySpace
{
	namespace Graphics
	{

		class CDepthShadow
		{
		private:
			//--- �����o�ϐ�
			std::shared_ptr<MySpace::Graphics::CDepthStencil> m_pDepthStencil;	// �������ݐ�
			std::shared_ptr<MySpace::Graphics::CRenderTarget> m_pRenderTarget;	// �������ݐ�
			float m_fNear;
			float m_fFar;
			DirectX::XMFLOAT2 m_vScreenSize;

		public:
			CDepthShadow();
			~CDepthShadow();

			void InitShader();

			// *@�`��O
			// *@data clear
			// *@Bind PS and VS
			void Begin();

			// *@�`���
			// *@�`���Đݒ�
			void End();

			// *@ø����ݒ�
			void SetUpTexture(UINT slot);

			ID3D11ShaderResourceView* GetResource() { return m_pRenderTarget->GetSRV(); }

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

#endif // !__DEPTH_SHADOW_H__

