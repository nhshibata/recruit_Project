//=========================================================
// [imageResource.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/Texture.h>
#include <Application/Application.h>
#include <DebugSystem/errorMessage.h>

using namespace MySpace::Graphics;


//==========================================================
// �R���X�g���N�^
//==========================================================
CImageResource::CImageResource()
	: m_pSRV(nullptr), m_pTex(nullptr)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CImageResource::~CImageResource()
{
	Unload();
}

//==========================================================
// �ǂݍ���
//==========================================================
bool CImageResource::Load(std::string name)
{
	ID3D11Device* device = Application::Get()->GetDevice();
	ID3D11DeviceContext* devicecontext = Application::Get()->GetDeviceContext();

	HRESULT hr = CreateTextureFromFile(device, name.c_str(), &m_pSRV);
	if (FAILED(hr))
	{
		Debug::CErrorMessage::DispErrorHandle(hr);
		return false;
	}
	
	return true;
}

//==========================================================
// �������
//==========================================================
void CImageResource::Unload()
{
	if (m_pSRV != nullptr) 
	{
		m_pSRV->Release();
		m_pSRV = nullptr;
	}

	if (m_pTex != nullptr) 
	{
		m_pTex->Release();
		m_pTex = nullptr;
	}
}