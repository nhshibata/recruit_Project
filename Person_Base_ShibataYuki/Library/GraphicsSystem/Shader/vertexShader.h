//=========================================================
// [vertexShader.h] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __VERTEX_SHADER_H__
#define __VERTEX_SHADER_H__

//--- インクルード部
#include <GraphicsSystem/Shader/GraphicsBase.h>
#include <d3d11.h>

namespace MySpace
{
	namespace Graphics
	{
		class CVertexShader : public CGraphicsBase
		{
		private:
			// 頂点シェーダー
			ID3D11VertexShader* m_Shader;
			// 頂点レイアウト
			ID3D11InputLayout* m_Layout;
		public:
			CVertexShader();
			CVertexShader(ID3D11VertexShader* pVer, ID3D11InputLayout* pLay);
			virtual ~CVertexShader();

			virtual void Bind(UINT slot = 0);
			
			HRESULT Make(std::string fileName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT size);
		};
	}
}

#endif //__VERTEX_SHADER_H__