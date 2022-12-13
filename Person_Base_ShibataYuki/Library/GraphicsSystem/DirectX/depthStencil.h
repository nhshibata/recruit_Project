//==========================================================
// [depthStencil.h]
//-------------------------
// 作成:2022/12/12
//-------------------------
// 授業内で実施した深度ステンシルを実装
//==========================================================

//--- インクルードガード
#ifndef __DEPTH_STENCIL_H__
#define __DEPTH_STENCIL_H__

//--- インクルード部
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDepthStencil : public CImageResource
		{
		private:
			//--- メンバ変数
			ID3D11DepthStencilView* m_pDSV;

		private:
			//--- メンバ関数
			HRESULT Create(UINT width, UINT height, bool useStencil);

		public:
			CDepthStencil(UINT width = 1280, UINT height = 720, bool useStencil = true);
			~CDepthStencil();

			virtual void Release();

			ID3D11DepthStencilView* GetView() const { return m_pDSV; };

			void Clear();
		};
	}
}

#endif // !__DEPTH_STENCIL_H__
