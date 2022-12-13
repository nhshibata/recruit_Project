//==========================================================
// [renderTarget.h]
//-------------------------
// �쐬:2022/12/12
//-------------------------
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		class CRenderTarget : public CImageResource
		{
		private:
			//--- �����o�ϐ�
			ID3D11RenderTargetView* m_pRTV;
		
		private:
			//--- �����o�֐�
			HRESULT Create(DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, UINT width = 1280, UINT height = 720);

		public:
			CRenderTarget(DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, UINT width = 1280, UINT height = 720);
			~CRenderTarget();

			virtual void Release();

			ID3D11RenderTargetView* GetView() const { return m_pRTV; };

			void Clear(float* color);
		};
	}
}

#endif // !__RENDER_TARGET_H__