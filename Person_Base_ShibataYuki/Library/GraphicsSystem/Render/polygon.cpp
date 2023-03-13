//=========================================================
// [mesh.cpp] 
// 作成:2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Render/polygon.h>
#include <GraphicsSystem/Shader/shader.h>
#include <GraphicsSystem/Render/mesh.h>

#include <Application/Application.h>
#include <Application/screen.h>

#include <DirectXMath.h>

using namespace MySpace;
using namespace MySpace::System;
using namespace MySpace::Graphics;

// シェーダに渡す値
struct SHADER_GLOBAL_POLYGON 
{
	XMMATRIX	mWorld;		// ワールド変換行列(転置行列)
	XMMATRIX	mView;		// ビュー変換行列(転置行列)
	XMMATRIX	mProj;		// 射影変換行列(転置行列)
	XMMATRIX	mTex;		// テクスチャ変換行列(転置行列)
};

// 静的メンバ
ID3D11ShaderResourceView*	CPolygon::m_pTexture;				// テクスチャへのポインタ

VERTEX_2D					CPolygon::m_vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

Vector3						CPolygon::m_vPos;					// ポリゴンの移動量
Vector3						CPolygon::m_vAngle;					// ポリゴンの回転量
Vector3						CPolygon::m_vScale;					// ポリゴンのサイズ
Color						CPolygon::m_vColor;					// ポリゴンの頂点カラー
bool						CPolygon::m_bInvalidate;			// 頂点データ更新フラグ

XMFLOAT2					CPolygon::m_vPosTexFrame;			// UV座標
XMFLOAT2					CPolygon::m_vSizeTexFrame;			// テクスチャ抽出サイズ

ID3D11Buffer*				CPolygon::m_pConstantBuffer;		// 定数バッファ
ID3D11Buffer*				CPolygon::m_pVertexBuffer;			// 頂点バッファ
ID3D11SamplerState*			CPolygon::m_pSamplerState;			// テクスチャ サンプラ
ID3D11VertexShader*			CPolygon::m_pVertexShader;			// 頂点シェーダ
ID3D11InputLayout*			CPolygon::m_pInputLayout;			// 頂点フォーマット
ID3D11PixelShader*			CPolygon::m_pPixelShader;			// ピクセルシェーダ

XMFLOAT4X4					CPolygon::m_mProj;					// 射影変換行列
XMFLOAT4X4					CPolygon::m_mView;					// ビュー変換行列
XMFLOAT4X4					CPolygon::m_mWorld;					// ワールド変換行列
XMFLOAT4X4					CPolygon::m_mTex;					// テクスチャ変換行列

//==========================================================
// ｼｪｰﾀﾞｰ初期化
//==========================================================
HRESULT CPolygon::InitShader(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;

	// シェーダ初期化
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

#if 0
	hr = LoadShader("VS_2D", "PS_2D",
		&m_pVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}
#endif // 0

	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	hr = ps->Make(CSO_PATH(PS_2D.cso));
	if (FAILED(hr))
		return hr;
	hr = vs->Make(CSO_PATH(VS_2D.cso), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	pSM->SetPS("PS_2D", ps);
	pSM->SetVS("VS_2D", vs);

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL_POLYGON);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &m_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	// 変換行列初期化
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&m_mProj,
		XMMatrixOrthographicLH((float)CScreen::GetWidth(), (float)CScreen::GetHeight(), 1.0f, 100.0f));
	XMStoreFloat4x4(&m_mTex, XMMatrixIdentity());
	m_mTex._44 = 0.0f;

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_vColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_bInvalidate = false;

	m_vPosTexFrame = XMFLOAT2(0.0f, 0.0f);
	m_vSizeTexFrame = XMFLOAT2(1.0f, 1.0f);

	// 頂点情報の作成
	hr = MakeVertex(pDevice);
	if (FAILED(hr)) {
		return hr;
	}
	
	return hr;
}

//==========================================================
// 終了処理
//==========================================================
void CPolygon::Fin(void)
{
	// テクスチャ サンプラの開放
	SAFE_RELEASE(m_pSamplerState);
	// 頂点バッファの解放
	SAFE_RELEASE(m_pVertexBuffer);
	// 定数バッファの解放
	SAFE_RELEASE(m_pConstantBuffer);
	// ピクセルシェーダ解放
	SAFE_RELEASE(m_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(m_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(m_pVertexShader);
}

//==========================================================
// 描画
//==========================================================
void CPolygon::Draw(ID3D11DeviceContext* pDeviceContext)
{
	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	// 移動
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&m_mWorld, mWorld);

	if (m_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// テクスチャ無し
		m_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertex();

	/*pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);*/
	{
		auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		pSM->BindPS("PS_2D");
		pSM->BindVS("VS_2D");
	}

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	SHADER_GLOBAL_POLYGON cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// プリミティブ形状をセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

//==========================================================
// 描画
//==========================================================
void CPolygon::Draw(ID3D11DeviceContext* pDeviceContext, std::string ps, std::string vs)
{
	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	// 移動
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&m_mWorld, mWorld);

	if (m_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// テクスチャ無し
		m_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertex();

	{
		auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		pSM->BindPS(ps);
		pSM->BindVS(vs);
	}
	//pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	SHADER_GLOBAL_POLYGON cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// プリミティブ形状をセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

//==========================================================
// 頂点の作成
//==========================================================
HRESULT CPolygon::MakeVertex(ID3D11Device* pDevice)
{
	// 頂点座標の設定
#if 1
	m_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
#else
	m_vertexWk[0].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertexWk[1].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	m_vertexWk[2].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertexWk[3].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);

#endif
	// 拡散反射光の設定
	m_vertexWk[0].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
	m_vertexWk[1].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
	m_vertexWk[2].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
	m_vertexWk[3].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);

	// テクスチャ座標の設定
	m_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	m_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	m_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	m_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	//MeshBufferSharedPtr mb = std::make_shared< CMeshBuffer>();
	//CMeshBuffer::Description desc;
	//desc.vtxSize = sizeof(m_vertexWk);
	//desc.isWrite = true;
	//desc.pVtx = &m_vertexWk[0];
	//CShaderManager::Get()

	// 頂点バッファ
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	//vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(m_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertexWk[0];

	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);

	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}

//==========================================================
// 頂点座標の設定
//==========================================================
void CPolygon::SetVertex(void)
{
	if (m_bInvalidate) {
		//頂点バッファの中身を埋める
		ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();
		HRESULT hr = S_OK;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// 拡散反射光の設定
			m_vertexWk[0].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
			m_vertexWk[1].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
			m_vertexWk[2].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
			m_vertexWk[3].diffuse = XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a);
			// 頂点データを上書き
			memcpy_s(msr.pData, sizeof(m_vertexWk), m_vertexWk, sizeof(m_vertexWk));
			// 頂点データをアンロックする
			pDeviceContext->Unmap(m_pVertexBuffer, 0);
			// フラグをクリア
			m_bInvalidate = false;
		}
	}
}

