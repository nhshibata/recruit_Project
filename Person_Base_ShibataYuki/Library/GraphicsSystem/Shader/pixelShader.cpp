//=========================================================
// [pixelShader.cpp] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

#define _CRT_SECURE_NO_WARNINGS
//--- �C���N���[�h��
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
	//-- �V�F�[�_�[���[�h --
	HRESULT hr = S_OK;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp) { return E_FAIL; }

	//-- �t�@�C���̒��g���������ɓǂݍ��� --
	char* pData = nullptr;
	//-- �t�@�C���̃f�[�^�T�C�Y�𒲂ׂ� --
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);	//�ړ��ʌ��o
	fseek(fp, 0, SEEK_SET);
	//-- �������m�ۂ��ēǂݍ��� --
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
	