//=========================================================
// [DXDevice.cpp] 
//---------------------------------------------------------
// �쐬: / /
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>

#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

using namespace MySpace::Graphics;

//==========================================================
// ������
//==========================================================
HRESULT CDXDevice::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full)
{
	HRESULT hr = S_OK;

#if 0
	//--- ��肭�������A�[�x����肭�����Ȃ�

	IDXGIFactory* factory;				// factory
	IDXGIAdapter* adapter;				// videocard
	IDXGIOutput* adapterOutput;			// monitor
	unsigned int numModes;
	unsigned int numerator = 60;			// ���q
	unsigned int denominator = 1;			// ����
	DXGI_MODE_DESC* displayModeList;
	m_Width = Width;
	m_Height = Height;

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,					// Windows Advanced Rasterizer
		D3D_DRIVER_TYPE_REFERENCE,
	};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	unsigned int createDeviceFlags = 0;

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,					// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,					// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,					// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,					// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,					// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1					// Direct9.1�Ή�GPU���x��
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

	// �X���b�v�`�F�C���̐ݒ�
	// �X���b�v�`�F�C���Ƃ́A�E�C���h�E�ւ̕\���_�u���o�b�t�@���Ǘ�����
	// �N���X�@�}���`�T���v�����O�A���t���b�V�����[�g���ݒ�ł���
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// �[���N���A
	sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	sd.BufferDesc.Width = Width;						// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = Height;						// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B �͈͂O�D�O����P�D�O)
	sd.BufferDesc.RefreshRate.Numerator = numerator;				// ���t���b�V�����[�g�i����j
	sd.BufferDesc.RefreshRate.Denominator = denominator;			// ���t���b�V�����[�g�i���q�j
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
	sd.OutputWindow = hWnd;			// �֘A�t����E�C���h�E
	sd.SampleDesc.Count = 1;		// �}���`�T���v���̐�
	sd.SampleDesc.Quality = 0;		// �}���`�T���v���̃N�I���e�B
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (full) 
		sd.Windowed = false;				// �E�C���h�E���[�h
	else 
		sd.Windowed = TRUE;				// �E�C���h�E���[�h
	
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ���[�h�̎����؂�ւ�

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL,				// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�i�m�t�k�k�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			m_DriverType,		// �f�o�C�X�h���C�o�̃^�C�v
			NULL,				// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,	// �f�o�C�X�t���O
			featureLevels,		// �@�\���x��
			numFeatureLevels,	// �@�\���x����
			D3D11_SDK_VERSION,	// 
			&sd,				// �X���b�v�`�F�C���̐ݒ�
			g_pSwapChain.GetAddressOf(),		// IDXGIDwapChain�C���^�t�F�[�X	
			g_pDevice.GetAddressOf(),			// ID3D11Device�C���^�t�F�[�X
			&m_FeatureLevel,					// �T�|�[�g����Ă���@�\���x��
			g_pDeviceContext.GetAddressOf());	// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;
	
#if !RT_DS_TEST
	// �����_�����O�^�[�Q�b�g���쐬
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
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
	//--- �������m��
	m_pRenderTarget = std::make_shared<Graphics::CRenderTarget>(g_pSwapChain.Get(), GetDevice());
	m_pDepthStencil = std::make_shared<Graphics::CDepthStencil>(this);
#endif // RT_DS_TEST

	// �X�e���V���X�e�[�g�쐬
	// Z�o�b�t�@
	D3D11_RASTERIZER_DESC rasterDesc;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// �X�e���V���ݒ�\���̏�����
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

	// stencil operation if pixel is�@back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	hr = g_pDevice->CreateDepthStencilState(&depthStencilDesc, g_pDSS[0].GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}

	// �f�o�C�X�R���e�L�X�g�փZ�b�g
	g_pDeviceContext->OMSetDepthStencilState(g_pDSS[0].Get(), 1);

#if !RT_DS_TEST
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// depthstencilview ������
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

	//--- �r���[�|�[�g��ݒ�
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
	//	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;		// ���C���t���[���ɂ������Ƃ�
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterrizer state from the description we just filled out 
	hr = g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[0].GetAddressOf());
	// ���X�^���C�U����
	rasterDesc.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[1].GetAddressOf());
	rasterDesc.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());
	if (FAILED(hr)) {
		return false;
	}

	//  set the rasterizer state
	//g_pDeviceContext->RSSetState(g_pRs[0].Get());

	// �u�����h �X�e�[�g����
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

	// �ݒ�ƍ쐬
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	//--- �[�x�X�e���V���X�e�[�g����
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
	
	//--- �T���v���[�X�e�[�g�ݒ�
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
	//--- ��肭�����Ȃ�

	// �X���b�v�`�F�C���̐ݒ�
	// �X���b�v�`�F�C���Ƃ́A�E�C���h�E�ւ̕\���_�u���o�b�t�@���Ǘ�����
	// �N���X�@�}���`�T���v�����O�A���t���b�V�����[�g���ݒ�ł���
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));						// �[���N���A
	sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	sd.BufferDesc.Width = Width;						// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = Height;						// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B �͈͂O�D�O����P�D�O)
	sd.BufferDesc.RefreshRate.Numerator = 60;				// ���t���b�V�����[�g�i����j
	sd.BufferDesc.RefreshRate.Denominator = 1;			// ���t���b�V�����[�g�i���q�j
	//sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
	sd.OutputWindow = hWnd;			// �֘A�t����E�C���h�E
	sd.SampleDesc.Count = 1;		// �}���`�T���v���̐�
	sd.SampleDesc.Quality = 0;		// �}���`�T���v���̃N�I���e�B
	//sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (full)
		sd.Windowed = false;				// �E�C���h�E���[�h
	else
		sd.Windowed = TRUE;				// �E�C���h�E���[�h

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
	// �����_�����O�^�[�Q�b�g���쐬
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
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
	//--- �������m��
	m_pRenderTarget = std::make_shared<Graphics::CRenderTarget>(g_pSwapChain.Get(), GetDevice());
	m_pDepthStencil = std::make_shared<Graphics::CDepthStencil>(this);
#endif // RT_DS_TEST

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// �X�e���V���ݒ�\���̏�����
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

	// stencil operation if pixel is�@back-facing
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

	// �[�x�X�e���V���X�e�[�g����
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);

	// �f�o�C�X�R���e�L�X�g�փZ�b�g
	g_pDeviceContext->OMSetDepthStencilState(g_pDSS[0].Get(), 1);

#if !RT_DS_TEST
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// depthstencilview ������
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

	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	// create the rasterrizer state from the description we just filled out 
	hr = g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[0].GetAddressOf());
	// ���X�^���C�U����
	rasterDesc.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[1].GetAddressOf());
	rasterDesc.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	g_pDevice->CreateRasterizerState(&rasterDesc, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());
	if (FAILED(hr))
	{
		return false;
	}

	//--- �r���[�|�[�g��ݒ�
	m_viewPort = std::make_shared<D3D11_VIEWPORT>();
	m_viewPort.get()->Width = static_cast<FLOAT>(Width);
	m_viewPort.get()->Height = static_cast<FLOAT>(Height);
	m_viewPort.get()->MinDepth = 0.0f;
	m_viewPort.get()->MaxDepth = 1.0f;
	m_viewPort.get()->TopLeftX = 0;
	m_viewPort.get()->TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, m_viewPort.get());

	// �u�����h �X�e�[�g����
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
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	//--- �T���v���[�X�e�[�g�ݒ�
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
	//--- ��肭����

	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
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

	// �o�b�N�o�b�t�@����
		// �����_�[�^�[�Q�b�g�r���[����
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	pBackBuffer = nullptr;

	// Z�o�b�t�@�p�e�N�X�`������
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

	// Z�o�b�t�@�^�[�Q�b�g�r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture.Get(),
										   &dsvd, g_pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
		return hr;

	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	g_pDeviceContext->OMSetRenderTargets(1, g_pRenderTargetView.GetAddressOf(), g_pDepthStencilView.Get());

	// �r���[�|�[�g�ݒ�
	//D3D11_VIEWPORT vp;
	m_viewPort = std::make_shared<D3D11_VIEWPORT>();
	m_viewPort->Width = (float)Width;
	m_viewPort->Height = (float)Height;
	m_viewPort->MinDepth = 0.0f;
	m_viewPort->MaxDepth = 1.0f;
	m_viewPort->TopLeftX = 0;
	m_viewPort->TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, m_viewPort.get());


	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[0].GetAddressOf());
	rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[1].GetAddressOf());
	rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, g_pRs[2].GetAddressOf());
	g_pDeviceContext->RSSetState(g_pRs[2].Get());

	// �u�����h �X�e�[�g����
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
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[1].GetAddressOf());
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[2].GetAddressOf());
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, g_pBlendState[3].GetAddressOf());
	SetBlendState((int)EBlendState::BS_ALPHABLEND);

	// �[�x�X�e���V���X�e�[�g����
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
// �������
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

	// ����ɉ������Ɠ{����
#if 0
	// �����I�ȉ��
	g_pDevice.GetMain()->Release();				// �f�o�C�X
	g_pDeviceContext.GetMain()->Release();		// �f�o�C�X �R���e�L�X�g
	g_pSwapChain.GetMain()->Release();			// �X���b�v�`�F�[��
	g_pRenderTargetView.GetMain()->Release();	// �t���[���o�b�t�@
	g_pDepthStencilTexture.GetMain()->Release();	// Z�o�b�t�@�p������
	g_pDepthStencilView.GetMain()->Release();	// Z�o�b�t�@
	for (int cnt = 0; cnt < (int)ECullMode::MAX_CULLMODE; ++cnt)
	{
		g_pRs[cnt].GetMain()->Release();	// ���X�^���C�U �X�e�[�g
	}
	for (int cnt = 0; cnt < (int)EBlendState::MAX_BLENDSTATE; ++cnt)
	{
		if(g_pBlendState[cnt].GetMain())
			g_pBlendState[cnt].GetMain()->Release();// �u�����h �X�e�[�g
	}
	g_pDSS[0].GetMain()->Release();				// Z/�X�e���V�� �X�e�[�g
	g_pDSS[1].GetMain()->Release();				// Z/�X�e���V�� �X�e�[�g
	m_viewPort.reset();
	if(m_SamplerState.GetMain())m_SamplerState.GetMain()->Release();


	if(g_pDevice)g_pDevice.Reset();				// �f�o�C�X
	if(g_pDeviceContext)g_pDeviceContext.Reset();		// �f�o�C�X �R���e�L�X�g
	if(g_pSwapChain)g_pSwapChain.Reset();			// �X���b�v�`�F�[��
	if(g_pRenderTargetView)g_pRenderTargetView.Reset();	// �t���[���o�b�t�@
	if(g_pDepthStencilTexture)g_pDepthStencilTexture.Reset();	// Z�o�b�t�@�p������
	if(g_pDepthStencilView)g_pDepthStencilView.Reset();	// Z�o�b�t�@
	//for (int cnt = 0; cnt < (int)ECullMode::MAX_CULLMODE; ++cnt)
	//{
	//	g_pRs[cnt].Reset();	// ���X�^���C�U �X�e�[�g
	//}
	for (int cnt = 0; cnt < (int)EBlendState::MAX_BLENDSTATE; ++cnt)
	{
		if(g_pBlendState[cnt])
			g_pBlendState[cnt].Reset();// �u�����h �X�e�[�g
	}
	if(g_pDSS[0])g_pDSS[0].Reset();				// Z/�X�e���V�� �X�e�[�g
	if(g_pDSS[1])g_pDSS[1].Reset();				// Z/�X�e���V�� �X�e�[�g
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
