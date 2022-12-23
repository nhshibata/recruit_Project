//=========================================================
// [mesh.cpp] 
// 作成:2022/06/27
//---------------------------------------------------------
//=========================================================

#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Shader/shader.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <CoreSystem/Math/MyMath.h>

using namespace MySpace::Game;
using namespace MySpace::System;
using namespace MySpace::Graphics;
//using namespace DirectX;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define M_DIFFUSE				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR				MySpace::MyMath::Vector4(0.1f,0.1f,0.1f,1.0f)
#define M_POWER					(0.0f)
#define M_AMBIENT				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE				MySpace::MyMath::Vector4(0.0f,0.0f,0.0f,1.0f)
#define MAX_MESH_INSTANCING		1024

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// シェーダに渡す値
struct SHADER_GLOBAL {
	DirectX::XMMATRIX	mVP;		// ワールド×ビュー×射影行列(転置行列)
	DirectX::XMMATRIX	mW;			// ワールド行列(転置行列)
	DirectX::XMMATRIX	mTex;		// テクスチャ行列(転置行列)
};
struct SHADER_GLOBAL2 {
	XMVECTOR	vEye;		// 視点座標
	// 光源
	XMVECTOR	vLightDir;	// 光源方向
	XMVECTOR	vLa;		// 光源色(アンビエント)
	XMVECTOR	vLd;		// 光源色(ディフューズ)
	XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	XMVECTOR	vAmbient;	// アンビエント色(+テクスチャ有無)
	XMVECTOR	vDiffuse;	// ディフューズ色
	XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	XMVECTOR	vEmissive;	// エミッシブ色
};

struct SHADER_MESH_INSTANCING
{
	DirectX::XMMATRIX mWorld[MAX_MESH_INSTANCING];
	SHADER_MESH_INSTANCING()
	{
		for (int i = 0; i < MAX_MESH_INSTANCING; i++)
		{
			this->mWorld[i] = XMMatrixIdentity();
		}
	}
};

// 静的メンバ
ID3D11Buffer* CMesh::m_pConstantBuffer[2];	// 定数バッファ
ID3D11SamplerState* CMesh::m_pSamplerState;	// テクスチャ サンプラ
ID3D11VertexShader* CMesh::m_pVertexShader;	// 頂点シェーダ
ID3D11InputLayout* CMesh::m_pInputLayout;	// 頂点フォーマット
ID3D11PixelShader* CMesh::m_pPixelShader;	// ピクセルシェーダ

// コンストラクタ
CMesh::CMesh()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
}

// デストラクタ
CMesh::~CMesh()
{
}

// シェーダ初期化
HRESULT CMesh::InitShader()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = CDXDevice::Get()->GetDevice();


	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
#if 1
	hr = LoadShader("Vertex", "Pixel", &m_pVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}
	hr = LoadShader("InstancingVertex", "Pixel", &m_pInstancingVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}
#else

	PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	ConstantBufferSharedPtr cb_sg = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_sg2 = std::make_shared<CConstantBuffer>();
	ps->Make(FORDER_DIR(Pixel.cso));
	vs->Make(FORDER_DIR(Vertex.cso), layout, _countof(layout));
	cb_sg->Make(sizeof(SHADER_GLOBAL), 0, CConstantBuffer::EType::Vertex);
	cb_sg2->Make(sizeof(SHADER_GLOBAL2), 0, CConstantBuffer::EType::Pixel);

	CShaderManager::Get()->SetConstantBuffer("SHADER_GLOBAL", cb_sg);
	CShaderManager::Get()->SetConstantBuffer("SHADER_GLOBAL2", cb_sg2);
	CShaderManager::Get()->SetVS("Vertex", vs);
	CShaderManager::Get()->SetPS("Pixel", ps);

#endif // 0

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[0]);
	if (FAILED(hr)) return hr;
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[1]);
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &m_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	// コンスタントバッファ インスタンシング用 作成
	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_MESH_INSTANCING);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	hr = pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBufferI);
	if (FAILED(hr))
		return false;

	return hr;
}

// シェーダ終了処理
void CMesh::FinShader()
{
	// 定数バッファの解放
	for (int i = 0; i < _countof(m_pConstantBuffer); ++i) {
		SAFE_RELEASE(m_pConstantBuffer[i]);
	}
	// ピクセルシェーダ解放
	SAFE_RELEASE(m_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(m_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pConstantBufferI);
}

// 初期化
HRESULT CMesh::Init(const VERTEX_3D vertexWk[], int nVertex, int indexWk[], int nIndex)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = CDXDevice::Get()->GetDevice();

	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	m_nNumVertex = nVertex;

	//MeshBufferSharedPtr mb = std::make_shared<CMeshBuffer>();
	//CMeshBuffer::Description desc;
	//desc.vtxSize = sizeof(VERTEX_3D);
	//desc.vtxCount = m_nNumVertex;
	//desc.isWrite = true;
	//desc.pVtx = vertexWk;
	//desc.idxCount = nIndex;
	//desc.idxSize = CMeshBuffer::INDEX::INDEX32;
	//desc.pIdx = indexWk;
	//desc.topology = CMeshBuffer::TOPOLOGY::TRIANGLE_LIST;
	//mb->Make(desc);
	//CShaderManager::Get()->SetMB("VERTEX_3D", mb);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX_3D) * m_nNumVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertexWk;
	hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	m_nNumIndex = nIndex;

	CD3D11_BUFFER_DESC ibd(m_nNumIndex * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indexWk;
	hr = pDevice->CreateBuffer(&ibd, &initData, &m_pIndexBuffer);

	// マテリアルの初期設定
	m_material.m_Diffuse = M_DIFFUSE;
	m_material.m_Ambient = M_AMBIENT;
	m_material.m_Specular = M_SPECULAR;
	m_material.m_Power = M_POWER;
	m_material.m_Emissive = M_EMISSIVE;

	return hr;
}

// 終了処理
void CMesh::Fin()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

// 描画
void CMesh::Draw(ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = CDXDevice::Get()->GetDeviceContext();
	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);
	//CShaderManager::Get()->BindPS("Pixel");
	//CShaderManager::Get()->BindVS("Vertex");

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// 追加
	//if(pTexture)
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	SHADER_GLOBAL cb;
	DirectX::XMMATRIX mtxWorld = XMLoadFloat4x4(&m_mWorld);
	CCamera* pCamera = CCamera::GetMain();
	//cb.mVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mVP = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	
	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL", &cb);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL");

	if (mWorld)
	{
		cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(mWorld));
	}
	else
	{
		XMFLOAT4X4 mTex(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
		DirectX::XMMATRIX mtxTex = XMLoadFloat4x4(&mTex);
		cb.mTex = XMMatrixTranspose(mtxTex);
	}
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetPos());
	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::Get());
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&m_material.m_Diffuse);
	cb2.vAmbient = XMVectorSet(m_material.m_Ambient.x, m_material.m_Ambient.y, m_material.m_Ambient.z, (pTexture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(m_material.m_Specular.x, m_material.m_Specular.y, m_material.m_Specular.z, m_material.m_Power);
	cb2.vEmissive = XMLoadFloat4(&m_material.m_Emissive);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);

	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL2", &cb2);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL2");

	// プリミティブ形状をセット
	static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(m_nNumIndex, 0, 0);
}

// 描画
void CMesh::DrawInstancing(std::vector<CMesh*> aMesh, ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = CDXDevice::Get()->GetDeviceContext();
	pDeviceContext->VSSetShader(m_pInstancingVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);
	//CShaderManager::Get()->BindPS("Pixel");
	//CShaderManager::Get()->BindVS("Vertex");

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &aMesh[0]->m_pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(aMesh[0]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// 追加
	if (pTexture)
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	SHADER_GLOBAL cb;
	//DirectX::XMMATRIX mtxWorld = XMLoadFloat4x4(&m_mWorld);
	CCamera* pCamera = CCamera::GetMain();
	//cb.mVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mVP = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	//cb.mW = XMMatrixTranspose(mtxWorld);
	
	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL", &cb);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL");

	if (mWorld)
	{
		cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(mWorld));
	}
	else
	{
		XMFLOAT4X4 mTex(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
		DirectX::XMMATRIX mtxTex = XMLoadFloat4x4(&mTex);
		cb.mTex = XMMatrixTranspose(mtxTex);
	}
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetPos());
	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::Get());
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&aMesh[0]->m_material.m_Diffuse);
	cb2.vAmbient = XMVectorSet(aMesh[0]->m_material.m_Ambient.x, aMesh[0]->m_material.m_Ambient.y, aMesh[0]->m_material.m_Ambient.z, (pTexture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(aMesh[0]->m_material.m_Specular.x, aMesh[0]->m_material.m_Specular.y, aMesh[0]->m_material.m_Specular.z, aMesh[0]->m_material.m_Power);
	cb2.vEmissive = XMLoadFloat4(&aMesh[0]->m_material.m_Emissive);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);

	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL2", &cb2);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL2");

	D3D11_MAPPED_SUBRESOURCE pData;
	int cnt = 0;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBufferI, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_MESH_INSTANCING si;
		// 本来ここで抜ける
		for (; cnt < aMesh.size(); ++cnt)
		{
			si.mWorld[cnt] = XMMatrixTranspose(XMLoadFloat4x4(&aMesh[cnt]->m_mWorld));
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&si, sizeof(SHADER_MESH_INSTANCING));
		pDeviceContext->Unmap(m_pConstantBufferI, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferI);

	// プリミティブ形状をセット
	static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	// ポリゴンの描画
	pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(aMesh[0]->m_nNumIndex), static_cast<UINT>(aMesh.size()), 0, 0, 0);
}

void CMesh::DrawInstancing(std::vector<XMFLOAT4X4> aWorld)
{
	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = CDXDevice::Get()->GetDeviceContext();
	pDeviceContext->VSSetShader(m_pInstancingVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);
	//CShaderManager::Get()->BindPS("Pixel");
	//CShaderManager::Get()->BindVS("Vertex");

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	//pDeviceContext->PSSetShaderResources(0, 1, NULL);

	SHADER_GLOBAL cb;
	//DirectX::XMMATRIX mtxWorld = XMLoadFloat4x4(&m_mWorld);
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;
	//cb.mVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mVP = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	//cb.mW = XMMatrixTranspose(mtxWorld);

	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL", &cb);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL");

	{
		XMFLOAT4X4 mTex(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
		DirectX::XMMATRIX mtxTex = XMLoadFloat4x4(&mTex);
		cb.mTex = XMMatrixTranspose(mtxTex);
	}
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);
	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetPos());
	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::Get());
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&m_material.m_Diffuse);
	cb2.vAmbient = XMVectorSet(m_material.m_Ambient.x, m_material.m_Ambient.y, m_material.m_Ambient.z, (nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(m_material.m_Specular.x, m_material.m_Specular.y, m_material.m_Specular.z, m_material.m_Power);
	cb2.vEmissive = XMLoadFloat4(&m_material.m_Emissive);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);

	//CShaderManager::Get()->ConstantWrite("SHADER_GLOBAL2", &cb2);
	//CShaderManager::Get()->BindCB("SHADER_GLOBAL2");

	// プリミティブ形状をセット
	static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	//--- データセット
	D3D11_MAPPED_SUBRESOURCE pData;
	int cntNum = 0;
	while (cntNum < aWorld.size())
	{
		int cnt = 0;
		if (SUCCEEDED(pDeviceContext->Map(m_pConstantBufferI, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_MESH_INSTANCING si;
			
			for (; cnt < 1024; ++cnt, ++cntNum)
			{
				if (cnt >= aWorld.size() || cntNum >= aWorld.size())
					break;
				si.mWorld[cnt] = XMMatrixTranspose(XMLoadFloat4x4(&aWorld[cntNum]));
			}
			memcpy_s(pData.pData, pData.RowPitch, (void*)&si, sizeof(SHADER_MESH_INSTANCING));
			pDeviceContext->Unmap(m_pConstantBufferI, 0);
		}
		pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferI);

		// ポリゴンの描画
		if(aWorld.size() > 1024)
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(cnt), 0, 0, 0);
		else
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(aWorld.size()), 0, 0, 0);
	}
}

// マテリアル設定
void CMesh::SetMaterial(const CMeshMaterial* pMaterial)
{
	if (pMaterial) {
		m_material = *pMaterial;
	}
	else {
		m_material.m_Diffuse = M_DIFFUSE;
		m_material.m_Ambient = M_AMBIENT;
		m_material.m_Specular = M_SPECULAR;
		m_material.m_Power = 0.0f;
		m_material.m_Emissive = M_EMISSIVE;
	}
}

