//=========================================================
// [pixelShader.cpp] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

#define _CRT_SECURE_NO_WARNINGS
//--- インクルード部
#include <GraphicsSystem/Shader/pixelShader.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::Graphics;

CPixelShader::CPixelShader()
{
		m_Shader = nullptr;
}
CPixelShader::~CPixelShader()
{
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = nullptr;
	}
}
void CPixelShader::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = CDXDevice::Get().GetDeviceContext();
	pDC->PSSetShader(m_Shader, nullptr, 0);
}
HRESULT CPixelShader::Make(std::string fileName)
{
	//-- シェーダーロード --
	HRESULT hr = S_OK;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp) { return E_FAIL; }

	//-- ファイルの中身をメモリに読み込み --
	char* pData = nullptr;
	//-- ファイルのデータサイズを調べる --
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);	//移動量検出
	fseek(fp, 0, SEEK_SET);
	//-- メモリ確保して読み込み --
	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	ID3D11Device* pDevice = CDXDevice::Get().GetDevice();
	hr = pDevice->CreatePixelShader(pData, fileSize, nullptr, &m_Shader);
	if (FAILED(hr)) {
		if (pData) delete[] pData;
		return hr;
	}

	if (pData)
		delete[] pData;

	return hr;
}
	