//==========================================================
// [bloom.h]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// �V�F�[�_�[�̖���������Ή�
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

#ifndef __BLOOM_H__
#define __BLOOM_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

namespace MySpace
{
	namespace Graphics
	{
		class CBloom : public CPostProcess
		{
		private:
			float m_fBlurPower;				// �u���[����
			CGaussianBlur m_Gauss;			// �K�E�X�u���[
			CRenderTarget m_pLuminnceRT;	// �P�x���o�p
			CRenderTarget m_pMainRT;		// �ŏI�`���

		public:
			CBloom();
			~CBloom();

			static HRESULT InitShader();

			void DrawSprite(ID3D11ShaderResourceView* originalTexture);

			ID3D11ShaderResourceView* GetResource()override;
			const float GetPower()const { return m_fBlurPower; }
			void SetPower(const float value) { m_fBlurPower = value; }

		};
		
	}
}

#endif // !__BLOOM_H__

