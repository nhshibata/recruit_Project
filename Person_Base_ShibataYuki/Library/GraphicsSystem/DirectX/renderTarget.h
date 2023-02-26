//==========================================================
// [renderTarget.h]
//---------------------------------------------------------
// 作成:2022/12/12
//---------------------------------------------------------
//==========================================================

//--- インクルードガード
#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

//--- インクルード部
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		class CRenderTarget : public CImageResource
		{
		private:
			//--- メンバ変数
			ID3D11RenderTargetView* m_pRTV;
		
		
		public:
			//--- メンバ関数
			CRenderTarget();
			CRenderTarget(DXGI_FORMAT format, UINT width, UINT height);
			CRenderTarget(IDXGISwapChain* swap, ID3D11Device* pDevice);
			CRenderTarget(const CRenderTarget&);
			~CRenderTarget();
			
			HRESULT Create(DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, UINT width = 1280, UINT height = 720);

			virtual void Release();
			ID3D11RenderTargetView* GetView() const { return m_pRTV; };
			void Clear(float* color);
			void Clear(float r, float g, float b, float a);
		};
	}
}

#endif // !__RENDER_TARGET_H__
