//=========================================================
// [hullShader.h] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

#ifndef __HULL_SHADER_H__
#define __HULL_SHADER_H__

//--- インクルード部
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CHullShader : public CGraphicsBase
		{
		private:
			ID3D11HullShader* m_pHullShader;
		public:
			CHullShader();
			~CHullShader();

			void Bind(UINT slot = 0);
			void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);
			HRESULT Make(void* pData, UINT size);
			HRESULT Make(void* pData, SIZE_T size);
		};
	}
}

#endif //!__HULL_SHADER_H__