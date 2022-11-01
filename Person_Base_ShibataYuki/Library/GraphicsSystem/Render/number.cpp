//=========================================================
//�쐬:2022/04/24
// �����\�� : ���׽
//=========================================================
//
////---�C���N���[�h��
//#include "number.h"
//#include "Texture.h"
#include <GraphicsSystem/Shader/shader.h>
#include <GraphicsSystem/Render/mesh.h>
//
//// �萔��`
//#define PATH_TEXTURE_NUMBER		L"data/texture/number001.png"
//#define COUNT_X_NUMBER			10
//#define COUNT_Y_NUMBER			1
//
//// �ÓI�����o�ϐ�
//CNumber* CNumber::m_pInstance;
//
////
//CNumber::CNumber()
//{
//	HRESULT hr = S_OK;
//	ID3D11Device* pDevice = GetDevice();
//
//	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_NUMBER, &m_pTexture);
//}
////
//CNumber::~CNumber()
//{
//
//}
////
//void CNumber::Update()
//{
//	m_numberData.clear();
//}
////
//void CNumber::Draw()
//{
//	ID3D11DeviceContext* pDC = GetDeviceContext();
//	std::vector<STNumberData>::iterator it;
//
//	SetPolygonTexture(m_pTexture);
//	SetPolygonFrameSize(1.0f / COUNT_X_NUMBER,
//		1.0f / COUNT_Y_NUMBER);
//	for (it = m_numberData.begin(); it < m_numberData.end(); ++it)
//	{
//		SetPolygonSize((*it).size.x, (*it).size.y);
//		(*it).pos.y -= (*it).size.y * 0.5f;
//		(*it).pos.x += (*it).size.x * ((*it).width - 0.5f);
//		for (; (*it).width > 0; --(*it).width, (*it).pos.x -= (*it).size.x) {
//			unsigned n = (*it).Number % 10;
//			(*it).Number /= 10;
//			SetPolygonPos((*it).pos.x, (*it).pos.y);
//			SetPolygonUV(
//				(n % COUNT_X_NUMBER) / (float)COUNT_X_NUMBER,
//				(n / COUNT_X_NUMBER) / (float)COUNT_Y_NUMBER);
//			DrawPolygon(pDC);
//		}
//	}
//	// ���ɖ߂�
//	SetPolygonUV(0.0f, 0.0f);
//	SetPolygonFrameSize(1.0f, 1.0f);
//}
////
//void CNumber::Set(XMFLOAT2 vPos,		// �\���ʒu(����)
//	unsigned uNumber,	// �\�����l
//	int nWidth,			// �\������
//	float fSizeX,		// 1�����̕�
//	float fSizeY)		// 1�����̍���
//{
//	STNumberData data;
//	data.pos = vPos;
//	data.Number = uNumber;
//	data.width = nWidth;
//	data.size = XMFLOAT2(fSizeX, fSizeY);
//	m_numberData.push_back(data);
//}