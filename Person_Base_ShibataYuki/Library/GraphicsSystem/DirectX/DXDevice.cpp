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


//=========================================================
// �R���X�g���N�^
//=========================================================
CDXDevice::CDXDevice()
	:g_uSyncInterval(0), m_Width(0), m_Height(0), m_DriverType(),
	m_FeatureLevel()
{
}

//==========================================================
// ������
//==========================================================
HRESULT CDXDevice::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full)
{
	HRESULT hr = S_OK;

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
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture.Get(), &dsvd, g_pDepthStencilView.GetAddressOf());
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


CDXDevice* CDXDevice::Get()
{
	static CDXDevice instance;
	return &instance;
}
