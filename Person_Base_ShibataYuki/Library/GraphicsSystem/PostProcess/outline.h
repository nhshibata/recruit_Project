//==========================================================
// [outline.h]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __OUTLINE_H__
#define __OUTLINE_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class COutline : public CPostProcess
		{
		private:
			CRenderTarget m_pMainRT;		// �ŏI�`���

		public:
			COutline();
			~COutline();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			ID3D11ShaderResourceView* GetResource()override;

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

#endif // !__OUTLINE_H__

