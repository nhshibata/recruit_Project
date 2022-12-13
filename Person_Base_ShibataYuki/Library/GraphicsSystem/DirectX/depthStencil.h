//==========================================================
// [depthStencil.h]
//-------------------------
// �쐬:2022/12/12
//-------------------------
// ���Ɠ��Ŏ��{�����[�x�X�e���V��������
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DEPTH_STENCIL_H__
#define __DEPTH_STENCIL_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDepthStencil : public CImageResource
		{
		private:
			//--- �����o�ϐ�
			ID3D11DepthStencilView* m_pDSV;

		private:
			//--- �����o�֐�
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
