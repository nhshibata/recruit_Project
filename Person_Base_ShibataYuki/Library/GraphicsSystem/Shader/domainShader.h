//=========================================================
// [domainShader.h] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

#ifndef __DOMAIN_SHADER_H__
#define __DOMAIN_SHADER_H__

//--- インクルード部
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDomainShader : public CGraphicsBase
		{
		private:
			ID3D11DomainShader* m_pDomainShader;
		public:
			CDomainShader();
			~CDomainShader();

			void Bind(UINT slot = 0);
			HRESULT Make(void* pData, UINT size);
			inline HRESULT Make(void* pData, SIZE_T size) { return Make(pData, static_cast<UINT>(size)); };

		};
	}
}

#endif //!__DOMAIN_SHADER_H__