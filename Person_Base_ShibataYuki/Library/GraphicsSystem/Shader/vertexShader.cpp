//=========================================================
// [vertexShader.cpp] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- 警告抑止
#define _CRT_SECURE_NO_WARNINGS

//--- インクルード部
#include <GraphicsSystem/Shader/vertexShader.h>
#include <Application/Application.h>
#include <DebugSystem/errorMessage.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CVertexShader::CVertexShader()
{
	m_Shader = nullptr;
	m_Layout = nullptr;
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CVertexShader::CVertexShader(ID3D11VertexShader* pVer, ID3D11InputLayout* pLay)
{
	m_Shader = pVer;
	m_Layout = pLay;
}

//==========================================================
// デストラクタ
//==========================================================
CVertexShader::~CVertexShader()
{
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = nullptr;
	}
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}
}

//==========================================================
// 割り当て
//==========================================================
void CVertexShader::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->VSSetShader(m_Shader, nullptr, slot);
	pDC->IASetInputLayout(m_Layout);
}

//==========================================================
// 構築
//==========================================================
HRESULT CVertexShader::Make(std::string fileName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT size)
{
	HRESULT hr = E_FAIL;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp) { return hr; }

	//-- ファイルの中身をメモリに読み込み
	char* pData = nullptr;
	//-- ファイルのデータサイズを調べる
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);	//移動量検出
	fseek(fp, 0, SEEK_SET);
	//-- メモリ確保して読み込み
	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	ID3D11Device* pDevice = Application::Get()->GetDevice();

	hr = pDevice->CreateVertexShader(pData, fileSize, nullptr, &m_Shader);
	if (FAILED(hr))
	{
		if (pData) delete[] pData;
		Debug::CErrorMessage::DispErrorHandle(hr);
		return hr;
	}

	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	//	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	//	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	//	{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	//};

	hr = pDevice->CreateInputLayout(layout, size, pData, fileSize, &m_Layout);

	if (pData) delete[] pData;

	return hr;
}

