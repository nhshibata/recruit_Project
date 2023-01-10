//=========================================================
// [hullShader.cpp] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Shader/hullShader.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

CHullShader::CHullShader()
	:m_pHullShader(nullptr)
{
}

CHullShader::~CHullShader()
{
	SAFE_RELEASE(m_pHullShader);
}

void CHullShader::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->HSSetShader(m_pHullShader, nullptr, 0);
}

void CHullShader::SetTexture(int texNum, ID3D11ShaderResourceView** ppTex)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	// シェーダにテクスチャを渡す
	pDC->PSSetShaderResources(0, texNum, ppTex);
}

HRESULT CHullShader::Make(void* pData, UINT size)
{
	HRESULT hr = S_OK;
	ID3D11Device* pD = Application::Get()->GetDevice();

	hr = pD->CreateHullShader(pData, size, NULL, &m_pHullShader);
	if (FAILED(hr)) { return E_FAIL; }
	return hr;
}

HRESULT CHullShader::Make(void* pData, SIZE_T size)
{
	return Make(pData, static_cast<UINT>(size));
}
