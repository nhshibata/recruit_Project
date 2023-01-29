//=========================================================
// [constantBuffer.cpp] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;
	
//==========================================================
// �R���X�g���N�^
//==========================================================
CConstantBuffer::CConstantBuffer()
	:m_pBuffer(nullptr), m_uSlot(0),m_eType (EType::MAX)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CConstantBuffer::~CConstantBuffer()
{
	if (m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = nullptr;
	}
}

//==========================================================
// ���蓖��
//==========================================================
void CConstantBuffer::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();

	UINT uSlot = slot;
	// �f�t�H���g�����łȂ����
	if (m_uSlot != 0)
	{
		uSlot = m_uSlot;
	}

	switch (m_eType)
	{
	case CConstantBuffer::EType::Vertex:
		pDC->VSSetConstantBuffers(uSlot, 1, &m_pBuffer);
		break;
	case CConstantBuffer::EType::Pixel:
		pDC->PSSetConstantBuffers(uSlot, 1, &m_pBuffer);
		break;
	case CConstantBuffer::EType::All:
		pDC->PSSetConstantBuffers(uSlot, 1, &m_pBuffer);
		pDC->VSSetConstantBuffers(uSlot, 1, &m_pBuffer);
		break;
	case CConstantBuffer::EType::MAX:
		break;
	default:
		break;
	}
}

//==========================================================
// ��������
//==========================================================
void CConstantBuffer::Write(void* data)
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->UpdateSubresource(m_pBuffer, 0, nullptr, data, 0, 0);
}

//==========================================================
// �\�z
//==========================================================
HRESULT CConstantBuffer::Make(UINT size, UINT slot, EType type, D3D11_SUBRESOURCE_DATA* init)
{
	HRESULT hr = S_OK;
	ID3D11Device * pD = Application::Get()->GetDevice();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = static_cast<UINT>(size) + (size % 16 == 0 ? 0 : 16 - size % 16);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	//-- ���ێ�
	m_eType = type;
	m_uSlot = slot;

	if (init)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		hr = pD->CreateBuffer(&bd, init, &m_pBuffer);
		return hr;
	}

	hr = pD->CreateBuffer(&bd, nullptr, &m_pBuffer);
	if (FAILED(hr))
		return hr;
	return hr;
}

