//=========================================================
// [DXTexture.cpp] 
// �쐬: 2022/06/24
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SHADER_H__
#define __SHADER_H__

// �x����\��
//#pragma warning(disable : 4996)

//--- �C���N���[�h��
#include <d3d11.h>

namespace MySpace
{
	namespace Graphics
	{
		HRESULT LoadVertexShader(LPCWSTR pwszVSFName, ID3D11VertexShader** ppVertexShader,
			ID3D11InputLayout** ppVertexLayout,
			const D3D11_INPUT_ELEMENT_DESC* pInpElemDesc = nullptr, UINT uNumElem = 0);
		HRESULT LoadPixelShader(LPCWSTR pwszPSFName, ID3D11PixelShader** ppPixelShader);
		HRESULT LoadShader(LPCWSTR pwszVSFName, LPCWSTR pwszPSFName,
			ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout, ID3D11PixelShader** ppPixelShader,
			const D3D11_INPUT_ELEMENT_DESC* pInpElemDesc = nullptr, UINT uNumElem = 0);
		HRESULT LoadVertexShader(LPCSTR pszVSFName, ID3D11VertexShader** ppVertexShader,
			ID3D11InputLayout** ppVertexLayout,
			const D3D11_INPUT_ELEMENT_DESC* pInpElemDesc = nullptr, UINT uNumElem = 0);
		HRESULT LoadPixelShader(LPCSTR pszPSFName, ID3D11PixelShader** ppPixelShader);
		HRESULT LoadShader(LPCSTR pszVSFName, LPCSTR pszPSFName,
			ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout,
			ID3D11PixelShader** ppPixelShader,
			const D3D11_INPUT_ELEMENT_DESC* pInpElemDesc = nullptr, UINT uNumElem = 0);
	}
}
#endif // !__DX_TEXTURE_H__
