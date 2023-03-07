//==========================================================
// [negative.h]
//---------------------------------------------------------
// 作成:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __NEGATIVE_H__
#define __NEGATIVE_H__

//--- インクルード部
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class CNegative : public CPostProcess
		{
		private:
			CRenderTarget m_pMainRT;		// 最終描画先
			float m_fNega;

		public:
			CNegative();
			~CNegative();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			ID3D11ShaderResourceView* GetResource()override;

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

#endif // !__NEGATIVE_H__