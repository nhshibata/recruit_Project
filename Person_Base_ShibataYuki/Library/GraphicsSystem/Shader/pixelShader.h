//=========================================================
// [pixelShader.h] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __PIXEL_SHADER_H__
#define __PIXEL_SHADER_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CPixelShader : public CGraphicsBase
		{
		private:
			//--- �����o�ϐ�
			ID3D11PixelShader* m_pShader;		// �s�N�Z���V�F�[�_

		public:
			//--- �����o�֐�
			CPixelShader();
			virtual ~CPixelShader();

			virtual void Bind(UINT slot = 0);
			virtual HRESULT Make(std::string);
		};
	}
}

#endif //__PIXEL_SHADER_H__