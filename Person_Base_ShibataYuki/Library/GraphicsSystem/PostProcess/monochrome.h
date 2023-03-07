//==========================================================
// [monochrome.h]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __MONOCHROME_H__
#define __MONOCHROME_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class CMonochrome : public CPostProcess
		{
		private:
			CRenderTarget m_pMainRT;		// �ŏI�`���
			ImageSharedPtr m_pToon;

		public:
			CMonochrome();
			~CMonochrome();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			ID3D11ShaderResourceView* GetResource()override;

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

#endif // !__MONOCHROME_H__