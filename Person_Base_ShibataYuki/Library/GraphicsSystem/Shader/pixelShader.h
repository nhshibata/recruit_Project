//=========================================================
// [pixelShader.h] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __PIXEL_SHADER_H__
#define __PIXEL_SHADER_H__

//--- インクルード部
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CPixelShader : public CGraphicsBase
		{
		private:
			ID3D11PixelShader* m_Shader;		// ピクセルシェーダ

		public:
			CPixelShader();
			virtual ~CPixelShader();

			virtual void Bind(UINT slot = 0);
			virtual HRESULT Make(std::string);
		};
	}
}

#endif //__PIXEL_SHADER_H__