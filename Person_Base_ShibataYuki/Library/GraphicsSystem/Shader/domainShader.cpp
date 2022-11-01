//=========================================================
// [domainShader.cpp] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Shader/domainShader.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::Graphics;

CDomainShader::CDomainShader()
{

}
CDomainShader::~CDomainShader()
{

}
void CDomainShader::Bind(UINT slot)
{
	auto pDC = CDXDevice::Get().GetDeviceContext();
	pDC->DSSetShader(m_pDomainShader, NULL, 0);
}
HRESULT CDomainShader::Make(void* pData, UINT size)
{
	HRESULT hr = S_OK;
	ID3D11Device* pD = CDXDevice::Get().GetDevice();

	hr = pD->CreateDomainShader(pData, size, NULL, &m_pDomainShader);

	if (FAILED(hr)) { return E_FAIL; }
	return hr;
}