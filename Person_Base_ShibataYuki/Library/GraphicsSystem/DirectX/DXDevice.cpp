//=========================================================
// [DXDevice.cpp] 
//---------------------------------------------------------
// 作成: / /
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>

#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

using namespace MySpace::Graphics;

//==========================================================
// 初期化
//==========================================================
HRESULT CDXDevice::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full)
{
	HRESULT hr = S_OK;

#if 0
	//--- 上手くいくが、深度が上手くいかない

	IDXGIFactory* factory;				// factory
	IDXGIAdapter* adapter;				// videocard
	IDXGIOutput* adapterOutput;			// monitor
	unsigned int numModes;
	unsigned int numerator = 60;			// 分子
	unsigned int denominator = 1;			// 分母
	DXGI_MODE_DESC* displayModeList;
	m_Width = Width;
	m_Height = Height;

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,					// Windows Advanced Rasterizer
		D3D_DRIVER_TYPE_REFERENCE,
	};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	unsigned int createDeviceFlags = 0;

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,					// DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,					// DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,					// DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,					// DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,					// DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1					// Direct9.1対応GPUレベル
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Create a DirectX graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr)) {
		return false;
	}

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr)) {
		return false;
	}

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr)) {
		return false;
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) {
		return false;
	}

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr)) {
		return false;
	}

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (unsigned int i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == Width) {
			if (displayModeList[i].Height == Height) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// スワップチェインの設定
	// スワップチェインとは、ウインドウへの表示ダブルバッファを管理する
	// クラス　マルチサンプリング、リフレッシュレートが設定できる
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// ゼロクリア
	sd.BufferCount = 1;									// バックバッファの数
	sd.BufferDesc.Width = Width;						// バックバッファの幅
	sd.BufferDesc.Height = Height;						// バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット(R,G,B 範囲０．０から１．０)
	sd.BufferDesc.RefreshRate.Numerator = numerator;				// リフレッシュレート（分母）
	sd.BufferDesc.RefreshRate.Denominator = denominator;			// リフレッシュレート（分子）
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用方法
	sd.OutputWindow = hWnd;			// 関連付けるウインドウ
	sd.SampleDesc.Count = 1;		// マルチサンプルの数
	sd.SampleDesc.Quality = 0;		// マルチサンプルのクオリティ
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (full) 
		sd.Windowed = false;				// ウインドウモード
	else 
		sd.Windowed = TRUE;				// ウインドウモード
	
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// モードの自動切り替え

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL,				// ディスプレイデバイスのアダプタ（ＮＵＬＬの場合最初に見つかったアダプタ）
			m_DriverType,		// デバイスドライバのタイプ
			NULL,				// ソフトウェアラスタライザを使用する場合に指定する
			createDeviceFlags,	// デバイスフラグ
			featureLevels,		// 機能レベル
			numFeatureLevels,	// 機能レベル数
			D3D11_SDK_VERSION,	// 
			&sd,				// スワップチェインの設定
			g_pSwapChain.GetAddressOf(),		// IDXGIDwapChainインタフェース	
			g_pDevice.GetAddressOf(),			// ID3D11Deviceインタフェース
			&m_FeatureLevel,					// サポートされている機能レベル
			g_pDeviceContext.GetAddressOf());	// デバイスコンテキスト
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;
	
#if !RT_DS_TEST
	// レンダリングターゲットを作成
	// バックバッファのポインタを取得
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, g_pRenderTargetView.GetAddressOf());
	pBackBuffer->Release();
	pBackBuffer = 0;
	if (FAILED(hr))
		return false;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = g_pDevice->CreateTexture2D(&depthBufferDesc, NULL, g_pDepthStencilTexture.GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}
#else
	//--- メモリ確保
	m_pRenderTarget = std::make_shared<Graphics::CRenderTarget>(g_pSwapChain.Get(), GetDevice());
	m_pDepthStencil = std::make_shared<Graphics::CDepthStencil>(this);
#endif // RT_DS_TEST

	// ステンシルステート作成
	// Zバッファ
	D3D11_RASTERIZER_DESC rasterDesc;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// ステンシル設定構造体初期化
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// set up the description of the stencl state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operation if pixel is　back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	hr = g_pDevice->CreateDepthStencilState(&depthStencilDesc, g_pDSS[0].GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}

	// デバイスコンテキストへセット
	g_pDeviceContext->OMSetDepthStencilState(g_pDSS[0].Get(), 1);

#if !RT_DS_TEST
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// depthstencilview 初期化
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture.Get(), &depthStencilViewDesc, g_pDepthStencilView.GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
	g_pDeviceContext->OMSetRenderTargets(1, g_pRenderTargetView.GetAddressOf(), g_pDepthStencilView.Get());
#else	
	auto rtAdress = m_pRenderTarget.get()->GetView();
	g_pDeviceContext->OMSetRenderTargets(1, &rtAdress, m_pDepthStencil.get()->GetView());
#endif // RT_DS_TEST

	//--- ビューポートを設定
	m_viewPort = std::make_shared<D3D11_VIEWPORT>();
	m_viewPort.get()->Width = static_cast<FLOAT>(Width);
	m_viewPort.get()->Height = static_cast<FLOAT>(Height);
	m_viewPort.get()->MinDepth = 0.0f;
	m_viewPort.get()->MaxDepth = 1.0f;
	m_viewPort.get()->TopLeftX = 0;
	m_viewPort.get()->TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, m_viewPort.get());

	// setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	//	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;		// ワイヤフレームにしたいとき
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterrizer state from the description we just filled out 
	hr = g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[0].GetAddressOf());
	// ラスタライザ生成
	rasterDesc.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[1].GetAddressOf());
	rasterDesc.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());
	if (FAILED(hr)) {
		return false;
	}

	//  set the rasterizer state
	//g_pDeviceContext->RSSetState(g_pRs[0].Get());

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[0].GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	// 設定と作成
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	//--- 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsd.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd, g_pDSS[0].GetAddressOf());
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	//dsd2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsd2.DepthFunc = D3D11_COMPARISON_LESS;
	dsd2.StencilEnable = FALSE;
	hr = g_pDevice->CreateDepthStencilState(&dsd2, g_pDSS[1].GetAddressOf());
	if (FAILED(hr)) 
		return hr;
	
	//--- サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	g_pDevice->CreateSamplerState(&samplerDesc, m_SamplerState.GetAddressOf());

	g_pDeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());

	SetZWrite(false);


#elif 0
	//--- 上手くいかない

	// スワップチェインの設定
	// スワップチェインとは、ウインドウへの表示ダブルバッファを管理する
	// クラス　マルチサンプリング、リフレッシュレートが設定できる
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// ゼロクリア
	sd.BufferCount = 1;									// バックバッファの数
	sd.BufferDesc.Width = Width;						// バックバッファの幅
	sd.BufferDesc.Height = Height;						// バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット(R,G,B 範囲０．０から１．０)
	sd.BufferDesc.RefreshRate.Numerator = 60;				// リフレッシュレート（分母）
	sd.BufferDesc.RefreshRate.Denominator = 1;			// リフレッシュレート（分子）
	//sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用方法
	sd.OutputWindow = hWnd;			// 関連付けるウインドウ
	sd.SampleDesc.Count = 1;		// マルチサンプルの数
	sd.SampleDesc.Quality = 0;		// マルチサンプルのクオリティ
	//sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (full)
		sd.Windowed = false;				// ウインドウモード
	else
		sd.Windowed = TRUE;				// ウインドウモード

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
									   nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &sd,
									   &g_pSwapChain, &g_pDevice, nullptr, &g_pDeviceContext);
	if (FAILED(hr))
		return hr;

#if !RT_DS_TEST
	// レンダリングターゲットを作成
	// バックバッファのポインタを取得
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, g_pRenderTargetView.GetAddressOf());
	pBackBuffer->Release();
	pBackBuffer = 0;
	if (FAILED(hr))
		return false;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = g_pDevice->CreateTexture2D(&depthBufferDesc, NULL, g_pDepthStencilTexture.GetAddressOf());
	if (FAILED(hr))
		return false;
#else
	//--- メモリ確保
	m_pRenderTarget = std::make_shared<Graphics::CRenderTarget>(g_pSwapChain.Get(), GetDevice());
	m_pDepthStencil = std::make_shared<Graphics::CDepthStencil>(this);
#endif // RT_DS_TEST

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// ステンシル設定構造体初期化
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// set up the description of the stencl state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operation if pixel is　back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	hr = g_pDevice->CreateDepthStencilState(&depthStencilDesc, g_pDSS[0].GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);

	// デバイスコンテキストへセット
	g_pDeviceContext->OMSetDepthStencilState(g_pDSS[0].Get(), 1);

#if !RT_DS_TEST
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// depthstencilview 初期化
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// create the depth stencil view
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture.Get(), &depthStencilViewDesc, g_pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
	g_pDeviceContext->OMSetRenderTargets(1, g_pRenderTargetView.GetAddressOf(), g_pDepthStencilView.Get());
#else	
	auto rtAdress = m_pRenderTarget.get()->GetView();
	g_pDeviceContext->OMSetRenderTargets(1, &rtAdress, m_pDepthStencil.get()->GetView());
#endif // RT_DS_TEST

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	// create the rasterrizer state from the description we just filled out 
	hr = g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[0].GetAddressOf());
	// ラスタライザ生成
	rasterDesc.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[1].GetAddressOf());
	rasterDesc.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());
	if (FAILED(hr))
	{
		return false;
	}

	//--- ビューポートを設定
	m_viewPort = std::make_shared<D3D11_VIEWPORT>();
	m_viewPort.get()->Width = static_cast<FLOAT>(Width);
	m_viewPort.get()->Height = static_cast<FLOAT>(Height);
	m_viewPort.get()->MinDepth = 0.0f;
	m_viewPort.get()->MaxDepth = 1.0f;
	m_viewPort.get()->TopLeftX = 0;
	m_viewPort.get()->TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, m_viewPort.get());

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[0]);
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	//--- サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	g_pDevice->CreateSamplerState(&samplerDesc, m_SamplerState.GetAddressOf());

	g_pDeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());

	SetZWrite(false);

	return S_OK;
#else
	//--- 上手くいく

	// デバイス、スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = Width;
	scd.BufferDesc.Height = Height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = !full;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
									   nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
									   &g_pSwapChain, &g_pDevice, nullptr, &g_pDeviceContext);
	if (FAILED(hr))
		return hr;

	// バックバッファ生成
		// レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	pBackBuffer = nullptr;

	// Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = Width;
	td.Height = Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = g_pDevice->CreateTexture2D(&td, nullptr, &g_pDepthStencilTexture);
	if (FAILED(hr))
		return hr;

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture.Get(),
										   &dsvd, g_pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// 各ターゲットビューをレンダーターゲットに設定
	g_pDeviceContext->OMSetRenderTargets(1, g_pRenderTargetView.GetAddressOf(), g_pDepthStencilView.Get());

	// ビューポート設定
	//D3D11_VIEWPORT vp;
	m_viewPort = std::make_shared<D3D11_VIEWPORT>();
	m_viewPort->Width = (float)Width;
	m_viewPort->Height = (float)Height;
	m_viewPort->MinDepth = 0.0f;
	m_viewPort->MaxDepth = 1.0f;
	m_viewPort->TopLeftX = 0;
	m_viewPort->TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, m_viewPort.get());


	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[0].GetAddressOf());
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[1].GetAddressOf());
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[0].GetAddressOf());
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[1].GetAddressOf());
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[2].GetAddressOf());
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[3].GetAddressOf());
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, g_pDSS[0].GetAddressOf());
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	hr = g_pDevice->CreateDepthStencilState(&dsd2, g_pDSS[1].GetAddressOf());


#endif // 1

	return hr;
}

//==========================================================
// 解放処理
//==========================================================
void CDXDevice::Uninit()
{
	if (g_pDeviceContext.Get())
	{
		g_pDeviceContext.Get()->ClearState();
		g_pDeviceContext.Get()->OMSetRenderTargets(0, nullptr, nullptr);
	}

	if (g_pSwapChain.Get()) 
	{
		g_pSwapChain.Get()->SetFullscreenState(false, NULL);
	}

	// 勝手に解放すると怒られる
#if 0
	// 明示的な解放
	g_pDevice.GetMain()->Release();				// デバイス
	g_pDeviceContext.GetMain()->Release();		// デバイス コンテキスト
	g_pSwapChain.GetMain()->Release();			// スワップチェーン
	g_pRenderTargetView.GetMain()->Release();	// フレームバッファ
	g_pDepthStencilTexture.GetMain()->Release();	// Zバッファ用メモリ
	g_pDepthStencilView.GetMain()->Release();	// Zバッファ
	for (int cnt = 0; cnt < (int)ECullMode::MAX_CULLMODE; ++cnt)
	{
		g_pRs[cnt].GetMain()->Release();	// ラスタライザ ステート
	}
	for (int cnt = 0; cnt < (int)EBlendState::MAX_BLENDSTATE; ++cnt)
	{
		if(g_pBlendState[cnt].GetMain())
			g_pBlendState[cnt].GetMain()->Release();// ブレンド ステート
	}
	g_pDSS[0].GetMain()->Release();				// Z/ステンシル ステート
	g_pDSS[1].GetMain()->Release();				// Z/ステンシル ステート
	m_viewPort.reset();
	if(m_SamplerState.GetMain())m_SamplerState.GetMain()->Release();


	if(g_pDevice)g_pDevice.Reset();				// デバイス
	if(g_pDeviceContext)g_pDeviceContext.Reset();		// デバイス コンテキスト
	if(g_pSwapChain)g_pSwapChain.Reset();			// スワップチェーン
	if(g_pRenderTargetView)g_pRenderTargetView.Reset();	// フレームバッファ
	if(g_pDepthStencilTexture)g_pDepthStencilTexture.Reset();	// Zバッファ用メモリ
	if(g_pDepthStencilView)g_pDepthStencilView.Reset();	// Zバッファ
	//for (int cnt = 0; cnt < (int)ECullMode::MAX_CULLMODE; ++cnt)
	//{
	//	g_pRs[cnt].Reset();	// ラスタライザ ステート
	//}
	for (int cnt = 0; cnt < (int)EBlendState::MAX_BLENDSTATE; ++cnt)
	{
		if(g_pBlendState[cnt])
			g_pBlendState[cnt].Reset();// ブレンド ステート
	}
	if(g_pDSS[0])g_pDSS[0].Reset();				// Z/ステンシル ステート
	if(g_pDSS[1])g_pDSS[1].Reset();				// Z/ステンシル ステート
	if(m_viewPort)m_viewPort.reset();
	if(m_SamplerState)m_SamplerState.Reset();

#endif // 0
}

#if RT_DS_TEST

ID3D11RenderTargetView* CDXDevice::GetRenderTargetView() { return m_pRenderTarget.get()->GetView(); }

ID3D11DepthStencilView* CDXDevice::GetDepthStencilView() { return m_pDepthStencil.get()->GetView(); }

void CDXDevice::SwitchRender(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{
	auto rt = m_pRenderTarget.get()->GetView();
	GetDeviceContext()->OMSetRenderTargets(
		1,
		pRTV ? &pRTV : &rt,
		pDSV
	);
}

#endif // RT_DS_TEST
