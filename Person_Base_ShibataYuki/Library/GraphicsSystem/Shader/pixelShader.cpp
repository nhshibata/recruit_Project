//=========================================================
// [pixelShader.cpp] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- �x���}�~
#define _CRT_SECURE_NO_WARNINGS

//--- �C���N���[�h��
#include <GraphicsSystem/Shader/pixelShader.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CPixelShader::CPixelShader()
	:m_pShader(nullptr)
{
}

//==========================================================
// �f�X�g���N�^
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
// ���蓖��
//==========================================================
void CPixelShader::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->PSSetShader(m_pShader, nullptr, 0);
}

//==========================================================
// �\�z
//==========================================================
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
	