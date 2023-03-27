//==========================================================
// [gaussianBlur.cpp]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// シェーダーの魔導書から対応
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

//--- インクルード部
#include <math.h>
#include <d3d11.h>

#include <Application/Application.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

using namespace MySpace::Graphics;

#ifndef NAME_TO
#define NAME_TO(name)	#name
#endif // !NAME_TO

//=========================================================
// シェーダー設定
//=========================================================
HRESULT CGaussianBlur::InitShader()
{
	HRESULT hr = S_OK;
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	VertexShaderSharedPtr vsY = std::make_shared<CVertexShader>();
	VertexShaderSharedPtr vsX = std::make_shared<CVertexShader>();
	ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();

	hr = ps->Make(CPixelName::GetCSO(CPixelName::szBlur));
	if (FAILED(hr))
		return hr;
	pSM->SetPS(CPixelName::szBlur, ps);

	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = vsX->Make(CVertexName::GetCSO(CVertexName::szXBlur), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(CVertexName::szXBlur, vsX);

	hr = vsY->Make(CVertexName::GetCSO(CVertexName::szYBlur), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetVS(CVertexName::szYBlur, vsY);

	hr = cb->Make(sizeof(WEIGHT_TABLE), Slot::CB_BLUR, CConstantBuffer::EType::Pixel);
	if (FAILED(hr))
		return hr;
	pSM->SetCB(NAME_TO(WEIGHT_TABLE), cb);

	return hr;
}

//=========================================================
// コンストラクタ
//=========================================================
CGaussianBlur::CGaussianBlur()
{
	
}

//=========================================================
// 引数付きコンストラクタ
//=========================================================
CGaussianBlur::CGaussianBlur(
	ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT eFormat,
	float fWidth, float fHeight)
{
	Init(originalTexture, eFormat, fWidth, fHeight);
}

//=========================================================
// デストラクタ
//=========================================================
CGaussianBlur::~CGaussianBlur()
{
}

//=========================================================
// 初期化
//=========================================================
void CGaussianBlur::Init(
	ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT eFormat,
	float Width,
	float Height)
{
	m_originalTexture = originalTexture;
	m_fWidth = Width;
	m_fHeight = Height;
	m_eFormat = eFormat;

	//レンダリングターゲットを初期化。
	InitRenderTargets();	

	// 重みテーブルの計算
	CalcWeightsTableFromGaussian(m_weight.fWeights, NUM_WEIGHTS, 8.0f);
}

//=========================================================
// レンダーターゲットの初期化
//=========================================================
void CGaussianBlur::InitRenderTargets()
{
#if 0
	//Xブラー用のレンダリングターゲットを作成する。
	m_xBlurRenderTarget.Create(
		m_eFormat,
		(UINT)fWidth / 2,
		(UINT)fHeight
	);

	//Yブラー用のレンダリングターゲットを作成する。
	m_yBlurRenderTarget.Create(
		m_eFormat,
		(UINT)fWidth / 2,
		(UINT)fHeight / 2
	);
#else
	//Xブラー用のレンダリングターゲットを作成する。
	m_xBlurRenderTarget.Create(
		m_eFormat,
		(UINT)m_fWidth,
		(UINT)m_fHeight
	);

	//Yブラー用のレンダリングターゲットを作成する。
	m_yBlurRenderTarget.Create(
		m_eFormat,
		(UINT)m_fWidth,
		(UINT)m_fHeight
	);
#endif // 0

}

//=========================================================
// 描画
//=========================================================
void CGaussianBlur::ExecuteOnGPU(float blurPower, ID3D11ShaderResourceView* pTex)
{
	m_originalTexture = pTex;

	//--- 前準備
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetZBuffer(false);
	pDX->SetZWrite(false);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ADDITIVE));

	//重みテーブルを更新する
	UpdateWeightsTable(blurPower);

	//--- 横ブラーを実行
	//レンダリングターゲットを設定
	ID3D11RenderTargetView* pView[] = { m_xBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//レンダリングターゲットをクリア
	m_xBlurRenderTarget.Clear(0, 0, 0, 0);
	//ドロー
	DrawSpriteXBlur();

	//縦ブラーを実行
	//レンダリングターゲットを設定
	pView[0] = { m_yBlurRenderTarget.GetView() };
	pDX->GetDeviceContext()->OMSetRenderTargets(1, pView, nullptr);
	//レンダリングターゲットをクリア
	m_yBlurRenderTarget.Clear(0, 0, 0, 0);
	//ドロー
	DrawSpriteYBlur();

	//--- 設定を戻す
	pDX->SetZBuffer(true);
	pDX->SetZWrite(true);
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	pDX->GetDeviceContext()->RSSetViewports(1, pDX->GetViewPort());
}

//=========================================================
// X描画
//=========================================================
void CGaussianBlur::DrawSpriteXBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();	

	//横ブラー用のスプライトを初期化する
	{
		// スプライトの解像度はm_xBlurRenderTargetと同じ
		/*D3D11_VIEWPORT vp;
		vp.MaxDepth = 1;
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight;
		pDC->RSSetViewports(1, &vp);*/

		// ユーザー拡張の定数バッファにブラー用のパラメーターを設定する
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);
		pSM->BindCB(NAME_TO(WEIGHT_TABLE));
	}

	//--- 描画設定
	CPolygon::SetSize(m_fWidth, m_fHeight);
	CPolygon::SetPos(0, 0);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	// ﾃｸｽﾁｬ設定
	// 0番目
	CPolygon::SetTexture(m_originalTexture);
	//--- 描画
	CPolygon::Draw(pDC, CPixelName::szBlur, CVertexName::szXBlur);

	//--- 設定の初期化
	CPolygon::SetSize(1, 1);

}

//=========================================================
// Y描画
//=========================================================
void CGaussianBlur::DrawSpriteYBlur()
{
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	auto pDC = Application::Get()->GetDeviceContext();
	
	//縦ブラー用のスプライトを初期化する。
	{
		// スプライトの解像度はm_xBlurRenderTargetと同じ。
		/*D3D11_VIEWPORT vp;
		vp.MaxDepth = 1;
		vp.Width = m_fWidth / 2;
		vp.Height = m_fHeight / 2;
		pDC->RSSetViewports(1, &vp);*/

		// ユーザー拡張の定数バッファにブラー用のパラメーターを設定する。
		pSM->CBWrite(NAME_TO(WEIGHT_TABLE), &m_weight);
	}

	//--- 描画設定
	CPolygon::SetSize(m_fWidth, m_fHeight);
	// ﾃｸｽﾁｬ設定
	// 0番目
	CPolygon::SetTexture(m_xBlurRenderTarget.GetSRV());
	//--- 描画
	CPolygon::Draw(pDC, CPixelName::szBlur, CVertexName::szYBlur);

	//--- 設定の初期化
	CPolygon::SetSize(1, 1);
}

//=========================================================
// 重み計算
//=========================================================
void CGaussianBlur::UpdateWeightsTable(float blurPower)
{
	if (blurPower == 0.0f)
		return;

#if 1
	float total = 0.0f;
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		float x = static_cast<float>(i - NUM_WEIGHTS / 2);
		m_weight.fWeights[i] = expf(-(x * x) / (2.0f * blurPower));
		if (i == 0)
		{
			total += m_weight.fWeights[i];
		}
		else
		{
			total += 2.0f * m_weight.fWeights[i];
		}
	}
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] /= total;
	}

#else
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] = expf(-0.5f * (float)(i * i) / blurPower);
		total += 2.0f * m_weight.fWeights[i];

	}
	// 規格化
	for (int i = 0; i < NUM_WEIGHTS; i++)
	{
		m_weight.fWeights[i] /= total;
	}
#endif // 1

}


/// <summary>
/// ガウシアン関数を利用して重みテーブルを計算する
/// </summary>
/// <param name="weightsTbl">重みテーブルの記録先</param>
/// <param name="sizeOfWeightsTbl">重みテーブルのサイズ</param>
/// <param name="sigma">分散具合。この数値が大きくなると分散具合が強くなる</param>
void CGaussianBlur::CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{
	// 重みの合計を記録する変数を定義する
	float total = 0;

	// ここからガウス関数を用いて重みを計算している
	// ループ変数のxが基準テクセルからの距離
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// 重みの合計で除算することで、重みの合計を1にしている
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}