//==========================================================
// [postProcess.h]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// ���N���X
//==========================================================

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__

//--- �C���N���[�h��
#include <d3d11.h>
#include <GraphicsSystem/DirectX/renderTarget.h>

namespace MySpace
{
	namespace Graphics
	{
		class CGBuffer;
	}
}

namespace MySpace
{
	namespace Graphics
	{
		class CPostProcess
		{

		public:
			// *@�X�v���C�g�`��
			virtual void DrawSprite(CGBuffer* pGBuf) = 0;

			virtual ID3D11ShaderResourceView* GetResource() = 0;

#if BUILD_MODE
			virtual void ImGuiDebug() {};
#endif // BIULD_MODE

		};
	}
}

#endif // !__POST_PROCESS_H__


