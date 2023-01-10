//=========================================================
// [imageResource.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================
//--- インクルード部
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/Texture.h>
#include <Application/Application.h>

using namespace MySpace::Graphics;


//==========================================================
// コンストラクタ
//==========================================================
CImageResource::CImageResource()
	: m_pSRV(nullptr), m_pTex(nullptr)
{
}

//==========================================================
// デストラクタ
//==========================================================
CImageResource::~CImageResource()
{
	Unload();
}

//==========================================================
// 読み込み
//==========================================================
bool CImageResource::Load(std::string name)
{
	ID3D11Device* device = Application::Get()->GetDevice();
	ID3D11DeviceContext* devicecontext = Application::Get()->GetDeviceContext();

	HRESULT sts = CreateTextureFromFile(device, name.c_str(), &m_pSRV);
	
	if (FAILED(sts))
		return false;
	else 
		return true;
	
	return sts;
}

//==========================================================
// 解放処理
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