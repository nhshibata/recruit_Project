//=========================================================
// [FontTexture.cpp] 
// �쐬: 2022/08/04
// �X�V: 2022/09/13 �����Ɖp�������`��ł��Ȃ������̂��C��
//				wstring�Awchar�^�ɂ��邱�Ƃŉ���(cereal�͑Ή����ĂȂ��c)
//---------------------------------------------------------
// �قڂ��̕��Q�l
// https://hakase0274.hatenablog.com/entry/2018/11/17/170918
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Render/polygon.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CFontTexture::CFontTexture()
	:m_FontName(std::wstring())
{
	
}

//==========================================================
// �f�X�N�g���N�^
//==========================================================
CFontTexture::~CFontTexture()
{

}

//==========================================================
// ������
//==========================================================
HRESULT CFontTexture::Init()
{
	//���P�[����ݒ肵�Ȃ���Ε�����������\��������炵��
	setlocale(LC_ALL, "Japanese");
	m_aStringMap[L"�l�r �o����"];

#pragma region Convert
	const size_t WCHARBUF = 100;
	std::string name("aasdfg");
	std::wstring wname = std::wstring(name.begin(), name.end());
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name.data(), -1, wname.data(), WCHARBUF);

	// �m�F
	// char -> wchar �ϊ���������
	//std::string name("aasdfg");
	//std::wstring wname = std::wstring(name.begin(), name.end());
	//if (wname.empty()){}
	//
	//// wchar -> char �ϊ���肭�s��
	//std::wstring wwname(L"aasdfg");
	//std::string nname = std::string(name.begin(), name.end());
	//if (nname.empty()) {}
#pragma endregion

	return S_OK;
}

//==========================================================
// ���
//==========================================================
void CFontTexture::Uninit()
{
	for (auto & font : m_aStringMap)
	{	// vector
		for (auto & str : font.second)
		{
			str.second.pTex->Release();
		}
	}
	m_aStringMap.clear();

	SelectObject(m_hdc, m_hOldFont);
	DeleteObject(m_hFont);

	/*DESIGNVECTOR design;
	RemoveFontResourceEx(
		TEXT("�l�r �o����"),
		FR_PRIVATE,
		&design
	);*/
}

//==========================================================
// �t�H���g�쐬
//==========================================================
void CFontTexture::DesignFont(std::string fontPath, std::wstring fontName)
{
	// �t�H���g�n���h���̐���
	int fontSize = 64;
	int fontWeight = 64;

	SelectObject(m_hdc, m_hOldFont);
	DeleteObject(m_hFont);
	
	// �t�H���g���
	/*if(m_FontName.size() != static_cast <size_t>(0))
	{
		DESIGNVECTOR design;
		RemoveFontResourceEx(
			TEXT(m_FontName.c_str()),
			FR_PRIVATE,
			&design
		);
	}*/

	//-- �t�H���g�ǂݍ��� --
	DESIGNVECTOR design;
	AddFontResourceEx(
		TEXT(fontPath.c_str()),
		FR_PRIVATE,
		&design
	);
	
	LOGFONTW lf =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN
		//,(WCHAR)fontPath.c_str()
	};
	wcscpy_s(lf.lfFaceName, fontName.c_str());
	//strcpy(lf.lfFaceName, fontName.c_str());
	m_hFont = CreateFontIndirectW(&lf);

	// ���݂̃E�B���h�E�ɓK�p
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	m_hdc = GetDC(NULL);
	m_hOldFont = (HFONT)SelectObject(m_hdc, m_hFont);
}

//==========================================================
// ø�������
//==========================================================
CFontTexture::STCharaData CFontTexture::CreateTex(LPCWSTR character, std::wstring fontName)
{
	if (m_FontName != fontName)
	{
		// �t�H���g�n���h���̐���
		int fontSize = 64;
		int fontWeight = 64;
		// ��Ȃ�
		if (fontName.empty())
			fontName = L"�l�r �o����";

		LOGFONTW lf =
		{
			fontSize, 0, 0, 0, 
			fontWeight, 0, 0, 0,
			SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN
			//,(WCHAR)"�l�r �o����"
		};
		wcscpy_s(lf.lfFaceName, fontName.c_str());	//L"�l�r �o����"
		m_hFont = CreateFontIndirectW(&lf);

		// ���݂̃E�B���h�E�ɓK�p
		// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
		m_hdc = GetDC(NULL);
		m_hOldFont = (HFONT)SelectObject(m_hdc, m_hFont);
		m_FontName = fontName;
	}

	// �L�[�̊m�F
	if (m_aStringMap.count(fontName))
	{
		// �ǂݍ��ݍς�
		if (m_aStringMap[fontName].count(character))
		{
			return m_aStringMap[fontName][character];
		}
	}
	else
	{
		m_aStringMap[fontName];
	}

	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutlineW�֐��̓G���[�ƂȂ�B
	// �o�͂��镶��(�ꕶ������)
	UINT code = static_cast<UINT>(*character);

	// 17�K���̃O���[�̃O���t�r�b�g�}�b�v
	const UINT gradFlag = GGO_GRAY4_BITMAP;

	// �r�b�g�}�b�v�̐ݒ�
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	GetTextMetrics(m_hdc, &tm);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };

	// �t�H���g�r�b�g�}�b�v���擾
	DWORD size = GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE* pMono = new BYTE[size];
	GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, size, pMono, &mat);

	// �t�H���g�̕��ƍ���
	INT fontWidth = gm.gmCellIncX;
	INT fontHeight = tm.tmHeight;

	// �����_�[�^�[�Q�b�g�̐ݒ�
	D3D11_TEXTURE2D_DESC rtDesc;
	ZeroMemory(&rtDesc, sizeof(rtDesc));
	rtDesc.Width = fontWidth;
	rtDesc.Height = fontHeight;
	rtDesc.MipLevels = 1;
	rtDesc.ArraySize = 1;
	rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtDesc.SampleDesc.Count = 1;
	rtDesc.SampleDesc.Quality = 0;
	rtDesc.Usage = D3D11_USAGE_DYNAMIC;
	rtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	rtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	rtDesc.MiscFlags = 0;


	ID3D11Texture2D* fontTexture = 0;
	auto pApp = Application::Get();
	//�f�o�C�X�R���e�L�X�g
	auto deviceContext = pApp->GetDeviceContext();

	// �t�H���g�p�e�N�X�`�����쐬
	HRESULT hr = pApp->GetDevice()->CreateTexture2D(&rtDesc, NULL, &fontTexture);

	// �t�H���g�p�e�N�X�`�����\�[�X�Ƀe�N�X�`�������R�s�[
	D3D11_MAPPED_SUBRESOURCE mappedSubrsrc;
	deviceContext->Map(fontTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubrsrc);
	BYTE* pBits = static_cast<BYTE*>(mappedSubrsrc.pData);
	INT iOfs_x = gm.gmptGlyphOrigin.x;
	INT iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
	INT iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
	INT iBmp_h = gm.gmBlackBoxY;
	INT Level = 17;
	INT x, y;
	DWORD Alpha, Color;
	memset(pBits, 0, mappedSubrsrc.RowPitch * tm.tmHeight);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha = (255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy(static_cast<BYTE*>(pBits) + mappedSubrsrc.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	deviceContext->Unmap(fontTexture, 0);
	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)

	// ���������
	delete[] pMono;

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rtDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;

	//-- ���ۑ� --
	STCharaData newData;

	newData.Width = fontWidth;
	newData.Height = fontHeight;
	newData.OffSetX = iOfs_x;
	newData.OffSetY = iOfs_y;
	newData.BMPWidth = iBmp_w;
	newData.BMPHeight = iBmp_h;

	// �V�F�[�_���\�[�X�r���[���쐬
	pApp->GetDevice()->CreateShaderResourceView(fontTexture, &srvDesc, &newData.pTex);

	// �i�[
	m_aStringMap[fontName].insert(CharTexPair(character, newData));

	return newData;
#pragma region OLD
#if 0
	// �t�H���g�r�b�g�}�b�v�擾
	//const wchar_t* c = L"S";
	UINT code = static_cast<UINT>(character);
	const UINT gradFlag = GGO_GRAY4_BITMAP;
	// �K���̍ő�l
	int grad = 0;
	//switch (gradFlag)
	//{
	//case GGO_GRAY2_BITMAP:
	//	grad = 4;
	//	break;
	//case GGO_GRAY4_BITMAP:
	//	grad = 16;
	//	break;
	//case GGO_GRAY8_BITMAP:
	//	grad = 64;
	//	break;
	//}
	// �r�b�g�}�b�v�̐ݒ�
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	GetTextMetrics(m_hdc, &tm);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
	// �t�H���g�r�b�v�}�b�v���擾
	DWORD size = GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, size, pMono, &mat);

	//�t�H���g�̕��ƍ���
	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	//�t�H���g���������ރe�N�X�`���쐬
	D3D11_TEXTURE2D_DESC fontTextureDesc;
	ZeroMemory(&fontTextureDesc, sizeof(fontTextureDesc));
	fontTextureDesc.Width = fontWidth;
	fontTextureDesc.Height = fontHeight;
	fontTextureDesc.MipLevels = 1;
	fontTextureDesc.ArraySize = 1;
	fontTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	fontTextureDesc.SampleDesc.Count = 1;
	fontTextureDesc.SampleDesc.Quality = 0;
	fontTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	fontTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	fontTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fontTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* fontTexture = 0;
	auto pApp = CDXDevice::GetMain();
	HRESULT hr = pApp.GetDevice()->CreateTexture2D(&fontTextureDesc, NULL, &fontTexture);
	//�f�o�C�X�R���e�L�X�g
	auto deviceContext = pApp.GetDeviceContext();

	// �t�H���g�����e�N�X�`���ɏ������ޕ���
	// �t�H���g�pø���Resource�Ƀe�N�X�`�������R�s�[
	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = deviceContext->Map(
		fontTexture,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);

	// �����ŏ�������
	BYTE* pBits = static_cast<BYTE*>(hMappedResource.pData);
	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int iOfs_x = gm.gmptGlyphOrigin.x;
	int iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
	int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
	int iBmp_h = gm.gmBlackBoxY;
	int Level = 17;
	//int Level = grad;
	int x, y;
	DWORD Alpha, Color;
	memset(pBits, 0, hMappedResource.RowPitch * tm.tmHeight);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha =
				(255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)])
				/ (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy(
				(BYTE*)pBits
				+ hMappedResource.RowPitch * y + 4 * x,
				&Color,
				sizeof(DWORD));
		}
	}
	deviceContext->Unmap(fontTexture, 0);
	//�s�v�Ȃ̂ō폜
	delete[] pMono;


	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	//-- ���ۑ� --
	STCharaData newData;

	newData.Width = fontWidth;
	newData.Height = fontHeight;
	newData.OffSetX = iOfs_x;
	newData.OffSetY = iOfs_y;
	newData.BMPWidth = iBmp_w;
	newData.BMPHeight = iBmp_h;

	pApp.GetDevice()->CreateShaderResourceView(fontTexture, &srvDesc, &newData.pTex);

	// �i�[
	m_aStringMap[fontName].insert(CharTexPair(character, newData));

	return newData;

#endif // 0
#pragma endregion
	// PolygonManager��p����
	// �V�F�[�_�p�ɃT���v�����쐬����
	//D3D11_SAMPLER_DESC samDesc;
	//ZeroMemory(&samDesc, sizeof(samDesc));
	//samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samDesc.MaxAnisotropy = 1;
	//samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//CDXDevice::Get()->GetDevice()->CreateSamplerState(&samDesc, &mSampler);

}

//==========================================================
// �������񐶐�
//==========================================================
std::vector<CFontTexture::STCharaData> CFontTexture::GetString(std::wstring text, std::wstring fontName)
{
	std::vector<CFontTexture::STCharaData> ret;
	
	// ��������ꕶ�����؂���
	for (auto & character : text)
	{
		ret.push_back(CreateTex(static_cast<LPCWSTR>(&character), fontName));
	}

	return ret;
}