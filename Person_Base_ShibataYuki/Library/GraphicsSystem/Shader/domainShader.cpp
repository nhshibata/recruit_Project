//=========================================================
// [domainShader.cpp] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Shader/domainShader.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

CDomainShader::CDomainShader()
	:m_pDomainShader(nullptr)
{
}

CDomainShader::~CDomainShader()
{
	SAFE_RELEASE(m_pDomainShader);
}

void CDomainShader::Bind(UINT slot)
{
	auto pDC = Application::Get()->GetDeviceContext();
	pDC->DSSetShader(m_pDomainShader, NULL, 0);
}

HRESULT CDomainShader::Make(void* pData, UINT size)
{
	HRESULT hr = S_OK;
	ID3D11Device* pD = Application::Get()->GetDevice();

	hr = pD->CreateDomainShader(pData, size, NULL, &m_pDomainShader);

	if (FAILED(hr)) { return E_FAIL; }
	return hr;
}