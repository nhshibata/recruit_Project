//=========================================================
// [FontTexture.cpp] 
// 作成: 2022/08/04
// 更新: 2022/09/13 数字と英字しか描画できなかったのを修正
//				wstring、wchar型にすることで解決(cerealは対応してない…)
//---------------------------------------------------------
// ほぼこの方参考
// https://hakase0274.hatenablog.com/entry/2018/11/17/170918
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <GraphicsSystem/Render/polygon.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CFontTexture::CFontTexture()
	:m_FontName(std::wstring())
{
	
}

//==========================================================
// デスクトラクタ
//==========================================================
CFontTexture::~CFontTexture()
{

}

//==========================================================
// 初期化
//==========================================================
HRESULT CFontTexture::Init()
{
	//ロケールを設定しなければ文字化けする可能性があるらしい
	setlocale(LC_ALL, "Japanese");
	m_aStringMap[L"ＭＳ Ｐ明朝"];

#pragma region Convert
	const size_t WCHARBUF = 100;
	std::string name("aasdfg");
	std::wstring wname = std::wstring(name.begin(), name.end());
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name.data(), -1, wname.data(), WCHARBUF);

	// 確認
	// char -> wchar 変換文字化け
	//std::string name("aasdfg");
	//std::wstring wname = std::wstring(name.begin(), name.end());
	//if (wname.empty()){}
	//
	//// wchar -> char 変換上手く行く
	//std::wstring wwname(L"aasdfg");
	//std::string nname = std::string(name.begin(), name.end());
	//if (nname.empty()) {}
#pragma endregion

	return S_OK;
}

//==========================================================
// 解放
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
		TEXT("ＭＳ Ｐ明朝"),
		FR_PRIVATE,
		&design
	);*/
}

//==========================================================
// フォント作成
//==========================================================
void CFontTexture::DesignFont(std::string fontPath, std::wstring fontName)
{
	// フォントハンドルの生成
	int fontSize = 64;
	int fontWeight = 64;

	SelectObject(m_hdc, m_hOldFont);
	DeleteObject(m_hFont);
	
	// フォント解放
	/*if(m_FontName.size() != static_cast <size_t>(0))
	{
		DESIGNVECTOR design;
		RemoveFontResourceEx(
			TEXT(m_FontName.c_str()),
			FR_PRIVATE,
			&design
		);
	}*/

	//-- フォント読み込み --
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

	// 現在のウィンドウに適用
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	m_hdc = GetDC(NULL);
	m_hOldFont = (HFONT)SelectObject(m_hdc, m_hFont);
}

//==========================================================
// ﾃｸｽﾁｬ生成
//==========================================================
CFontTexture::STCharaData CFontTexture::CreateTex(LPCWSTR character, std::wstring fontName)
{
	if (m_FontName != fontName)
	{
		// フォントハンドルの生成
		int fontSize = 64;
		int fontWeight = 64;
		// 空なら
		if (fontName.empty())
			fontName = L"ＭＳ Ｐ明朝";

		LOGFONTW lf =
		{
			fontSize, 0, 0, 0, 
			fontWeight, 0, 0, 0,
			SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN
			//,(WCHAR)"ＭＳ Ｐ明朝"
		};
		wcscpy_s(lf.lfFaceName, fontName.c_str());	//L"ＭＳ Ｐ明朝"
		m_hFont = CreateFontIndirectW(&lf);

		// 現在のウィンドウに適用
		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		m_hdc = GetDC(NULL);
		m_hOldFont = (HFONT)SelectObject(m_hdc, m_hFont);
		m_FontName = fontName;
	}

	// キーの確認
	if (m_aStringMap.count(fontName))
	{
		// 読み込み済み
		if (m_aStringMap[fontName].count(character))
		{
			return m_aStringMap[fontName][character];
		}
	}
	else
	{
		m_aStringMap[fontName];
	}

	// デバイスにフォントを持たせないとGetGlyphOutlineW関数はエラーとなる。
	// 出力する文字(一文字だけ)
	UINT code = static_cast<UINT>(*character);

	// 17階調のグレーのグリフビットマップ
	const UINT gradFlag = GGO_GRAY4_BITMAP;

	// ビットマップの設定
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	GetTextMetrics(m_hdc, &tm);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };

	// フォントビットマップを取得
	DWORD size = GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE* pMono = new BYTE[size];
	GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, size, pMono, &mat);

	// フォントの幅と高さ
	INT fontWidth = gm.gmCellIncX;
	INT fontHeight = tm.tmHeight;

	// レンダーターゲットの設定
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
	//デバイスコンテキスト
	auto deviceContext = pApp->GetDeviceContext();

	// フォント用テクスチャを作成
	HRESULT hr = pApp->GetDevice()->CreateTexture2D(&rtDesc, NULL, &fontTexture);

	// フォント用テクスチャリソースにテクスチャ情報をコピー
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
	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)

	// メモリ解放
	delete[] pMono;

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = rtDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;

	//-- 情報保存 --
	STCharaData newData;

	newData.Width = fontWidth;
	newData.Height = fontHeight;
	newData.OffSetX = iOfs_x;
	newData.OffSetY = iOfs_y;
	newData.BMPWidth = iBmp_w;
	newData.BMPHeight = iBmp_h;

	// シェーダリソースビューを作成
	pApp->GetDevice()->CreateShaderResourceView(fontTexture, &srvDesc, &newData.pTex);

	// 格納
	m_aStringMap[fontName].insert(CharTexPair(character, newData));

	return newData;
#pragma region OLD
#if 0
	// フォントビットマップ取得
	//const wchar_t* c = L"S";
	UINT code = static_cast<UINT>(character);
	const UINT gradFlag = GGO_GRAY4_BITMAP;
	// 階調の最大値
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
	// ビットマップの設定
	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	GetTextMetrics(m_hdc, &tm);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
	// フォントビップマップを取得
	DWORD size = GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, 0, NULL, &mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(m_hdc, code, gradFlag, &gm, size, pMono, &mat);

	//フォントの幅と高さ
	int fontWidth = gm.gmCellIncX;
	int fontHeight = tm.tmHeight;

	//フォントを書き込むテクスチャ作成
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
	//デバイスコンテキスト
	auto deviceContext = pApp.GetDeviceContext();

	// フォント情報をテクスチャに書き込む部分
	// フォント用ﾃｸｽﾁｬResourceにテクスチャ情報をコピー
	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = deviceContext->Map(
		fontTexture,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);

	// ここで書き込む
	BYTE* pBits = static_cast<BYTE*>(hMappedResource.pData);
	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
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
	//不要なので削除
	delete[] pMono;


	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = fontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

	//-- 情報保存 --
	STCharaData newData;

	newData.Width = fontWidth;
	newData.Height = fontHeight;
	newData.OffSetX = iOfs_x;
	newData.OffSetY = iOfs_y;
	newData.BMPWidth = iBmp_w;
	newData.BMPHeight = iBmp_h;

	pApp.GetDevice()->CreateShaderResourceView(fontTexture, &srvDesc, &newData.pTex);

	// 格納
	m_aStringMap[fontName].insert(CharTexPair(character, newData));

	return newData;

#endif // 0
#pragma endregion
	// PolygonManagerを用いる
	// シェーダ用にサンプラを作成する
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
// 文字列情報生成
//==========================================================
std::vector<CFontTexture::STCharaData> CFontTexture::GetString(std::wstring text, std::wstring fontName)
{
	std::vector<CFontTexture::STCharaData> ret;
	
	// 文字列を一文字ずつ切り取る
	for (auto & character : text)
	{
		ret.push_back(CreateTex(static_cast<LPCWSTR>(&character), fontName));
	}

	return ret;
}