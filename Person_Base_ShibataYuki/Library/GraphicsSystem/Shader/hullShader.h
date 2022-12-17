//=========================================================
// [hullShader.h] 
// �쐬: 2022/08/05
//---------------------------------------------------------
//=========================================================

#ifndef __HULL_SHADER_H__
#define __HULL_SHADER_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CHullShader : public CGraphicsBase
		{
		private:
			//--- �����o�ϐ�
			ID3D11HullShader* m_pHullShader;

		public:
			//--- �����o�֐�
			CHullShader();
			~CHullShader();

			void Bind(UINT slot = 0);
			HRESULT Make(void* pData, UINT size);
			HRESULT Make(void* pData, SIZE_T size);
			void SetTexture(int texNum, ID3D11ShaderResourceView** ppTex);
		};
	}
}

#endif //!__HULL_SHADER_H__