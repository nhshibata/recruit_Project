//=========================================================
// [imageResource.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================
//--- インクルード部
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/Texture.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

CImageResource::CImageResource()
	: m_pSRV(nullptr), m_pTex(nullptr)
{
	//res = nullptr;
}
CImageResource::~CImageResource()
{
	Unload();
}

bool CImageResource::Load(std::string name)
{
	ID3D11Device* device = CDXDevice::Get()->GetDevice();
	ID3D11DeviceContext* devicecontext = CDXDevice::Get()->GetDeviceContext();

	//bool sts = CreateSRVfromFile(name.c_str(), device, devicecontext, &res, &srv);
	HRESULT sts = CreateTextureFromFile(device, name.c_str(), &m_pSRV);
	// miss
	if (FAILED(sts)){
		return false;
	}
	else {
		return true;
	}
	return sts;
}

void CImageResource::Unload()
{
	if (m_pSRV != nullptr) {
		m_pSRV->Release();
		m_pSRV = nullptr;
	}
	if (m_pTex != nullptr) {
		m_pTex->Release();
		m_pTex = nullptr;
	}
}