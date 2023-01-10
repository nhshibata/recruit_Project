//=========================================================
// [pixelShader.cpp] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- 警告抑止
#define _CRT_SECURE_NO_WARNINGS

//--- インクルード部
#include <GraphicsSystem/Shader/pixelShader.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CPixelShader::CPixelShader()
	:m_pShader(nullptr)
{
}

//==========================================================
// デストラクタ
//==========================================================
CPixelShader::~CPixelShader()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}
}

//==========================================================
// 割り当て
//==========================================================
void CPixelShader::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->PSSetShader(m_pShader, nullptr, 0);
}

//==========================================================
// 構築
//==========================================================
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

	ID3D11Device* pDevice = Application::Get()->GetDevice();
	hr = pDevice->CreatePixelShader(pData, fileSize, nullptr, &m_pShader);
	if (FAILED(hr)) {
		if (pData) delete[] pData;
		return hr;
	}

	if (pData)
		delete[] pData;

	return hr;
}
	