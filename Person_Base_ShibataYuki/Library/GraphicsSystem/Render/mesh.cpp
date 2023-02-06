//=========================================================
// [mesh.cpp] 
//---------------------------------------------------------
// 作成:2022/06/27
// 更新:2023/02/06 シェーダーを完全に切替
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <Application/Application.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Shader/shader.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Manager/assetsManager.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <CoreSystem/Math/MyMath.h>

using namespace MySpace::Game;
using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- マクロ定義
#define M_DIFFUSE				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,0.3f)
#define M_SPECULAR				MySpace::MyMath::Vector4(0.1f,0.1f,0.1f,1.0f)
#define M_POWER					(0.0f)
#define M_AMBIENT				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE				MySpace::MyMath::Vector4(0.0f,0.0f,0.0f,1.0f)

//--- 構造体定義
// シェーダに渡す値
struct SHADER_GLOBAL 
{
	DirectX::XMMATRIX	mVP;		// ワールド×ビュー×射影行列(転置行列)
	DirectX::XMMATRIX	mW;			// ワールド行列(転置行列)
	DirectX::XMMATRIX	mTex;		// テクスチャ行列(転置行列)
};

struct SHADER_GLOBAL2 
{
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

	void SetData(CCamera* pCam, CDirectionalLight* pLight, CMeshMaterial* pMat, ID3D11ShaderResourceView* pTex)
	{
		this->vEye = XMLoadFloat3(&pCam->GetPos());
		this->vLightDir = XMLoadFloat3(&pLight->GetDir());
		this->vLa = XMLoadFloat4(&pLight->GetAmbient());
		this->vLd = XMLoadFloat4(&pLight->GetDiffuse());
		this->vLs = XMLoadFloat4(&pLight->GetSpecular());
		this->vDiffuse = XMLoadFloat4(&pMat->m_Diffuse);
		this->vAmbient = XMVectorSet(pMat->m_Ambient.x, pMat->m_Ambient.y, pMat->m_Ambient.z, pTex ? 1.0f : 0.0f);
		this->vSpecular = XMVectorSet(pMat->m_Specular.x, pMat->m_Specular.y, pMat->m_Specular.z, pMat->m_Power);
		this->vEmissive = XMLoadFloat4(&pMat->m_Emissive);
	}
};


//--- 静的メンバ
ID3D11SamplerState* CMesh::m_pSamplerState;	// テクスチャ サンプラ

//==========================================================
// コンストラクタ
//==========================================================
CMesh::CMesh()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
}

//==========================================================
// デストラクタ
//==========================================================
CMesh::~CMesh()
{
}
//==========================================================
// シェーダ初期化
//==========================================================
HRESULT CMesh::InitShader()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = Application::Get()->GetDevice();
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// シェーダ初期化
	// inputLayout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

#if 0
	hr = LoadShader("Vertex", "Pixel", &m_pVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}
	hr = LoadShader("VS_Mesh", "PS_Mesh", &m_pInstancingVS, &m_pShadowIL, &m_pShadowPS, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}
#else
	//--- PS、VSの生成と登録
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(FORDER_DIR(Data/shader/PS_Mesh.cso));
		if (FAILED(hr))
			return hr;
		else
			sm->SetPS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS, ps);

		VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
		hr = vs->Make(FORDER_DIR(Data/shader/VS_Mesh.cso), layout, _countof(layout));
		if (FAILED(hr))
			return hr;
		else
			sm->SetVS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS, vs);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(FORDER_DIR(Data/shader/Pixel.cso));
		if (FAILED(hr))
			return hr;
		else
			sm->SetPS(CMesh::SHADER_NAME_MESH_PSVS, ps);

		VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
		hr = vs->Make(FORDER_DIR(Data/shader/Vertex.cso), layout, _countof(layout));
		if (FAILED(hr))
			return hr;
		else
			sm->SetVS(CMesh::SHADER_NAME_MESH_PSVS, vs);
	}
	
#endif // 0

	//--- 定数バッファ生成
	ConstantBufferSharedPtr cb_sg = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_sg2 = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_im = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_imtx = std::make_shared<CConstantBuffer>();
	cb_sg->Make(sizeof(SHADER_GLOBAL), 0, CConstantBuffer::EType::Vertex);
	cb_sg2->Make(sizeof(SHADER_GLOBAL2), 1, CConstantBuffer::EType::Pixel);
	cb_im->MakeCPU(sizeof(INSTANCHING_DATA), 6, CConstantBuffer::EType::Pixel);
	cb_imtx->MakeCPU(sizeof(INSTANCE_MATRIX), 3, CConstantBuffer::EType::Vertex);
	sm->SetCB(typeid(SHADER_GLOBAL).name(), cb_sg);
	sm->SetCB(typeid(SHADER_GLOBAL2).name(), cb_sg2);
	sm->SetCB(CB_NAME_INSTANCE_MATERIAL, cb_im);
	sm->SetCB(CB_NAME_INSTANCE_MATRIX, cb_imtx);

	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(SHADER_GLOBAL);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = 0;
	//hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[0]);
	//if (FAILED(hr)) return hr;
	//
	//bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	//hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[1]);
	//if (FAILED(hr)) return hr;


	//// コンスタントバッファ インスタンシング用 作成
	//D3D11_BUFFER_DESC cb;
	//ZeroMemory(&cb, sizeof(cb));
	//cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cb.ByteWidth = sizeof(INSTANCHING_DATA);
	//cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.MiscFlags = 0;
	//cb.Usage = D3D11_USAGE_DYNAMIC;
	//hr = pDevice->CreateBuffer(&cb, nullptr, &m_pConstantBufferI);
	//if (FAILED(hr))
	//	return false;

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
	
	return hr;
}

//==========================================================
// シェーダ終了処理
//==========================================================
void CMesh::FinShader()
{
	// 定数バッファの解放
	/*for (int i = 0; i < _countof(m_pConstantBuffer); ++i) {
		SAFE_RELEASE(m_pConstantBuffer[i]);
	}*/
	//SAFE_RELEASE(m_pConstantBufferI);
	
	// ピクセルシェーダ解放
	//SAFE_RELEASE(m_pPixelShader);
	//SAFE_RELEASE(m_pShadowPS);
	//// 頂点フォーマット解放
	//SAFE_RELEASE(m_pInputLayout);
	//SAFE_RELEASE(m_pShadowIL);
	//// 頂点シェーダ解放
	//SAFE_RELEASE(m_pVertexShader);
	//SAFE_RELEASE(m_pInstancingVS);

	SAFE_RELEASE(m_pSamplerState);
	
}

//==========================================================
// 初期化
//==========================================================
HRESULT CMesh::Init(const VERTEX_3D vertexWk[], int nVertex, int indexWk[], int nIndex)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = Application::Get()->GetDevice();

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

//==========================================================
// 終了処理
//==========================================================
void CMesh::Fin()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

//==========================================================
// 描画
//==========================================================
void CMesh::Draw(ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();
	sm->BindPS(CMesh::SHADER_NAME_MESH_PSVS);
	sm->BindVS(CMesh::SHADER_NAME_MESH_PSVS);

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	
	if(pTexture)
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	SHADER_GLOBAL cb;
	DirectX::XMMATRIX mtxWorld = XMLoadFloat4x4(&m_mWorld);
	CCamera* pCamera = CCamera::GetMain();
	//cb.mVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mVP = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);

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
	/*pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);*/
	sm->CBWrite("SHADER_GLOBAL", &cb);
	sm->BindCB("SHADER_GLOBAL");

	SHADER_GLOBAL2 cb2;
	cb2.SetData(pCamera, dynamic_cast<CDirectionalLight*>(CLight::GetMain()), &m_material, pTexture);
	/*pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);*/
	sm->CBWrite("SHADER_GLOBAL2", &cb2);
	sm->BindCB("SHADER_GLOBAL2");
	

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

//==========================================================
// 描画インスタンシング
//==========================================================
void CMesh::DrawInstancing(std::vector<RENDER_DATA> aData, bool defaultShader,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	if (defaultShader)
	{
		// シェーダ設定
		/*ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();
		pDeviceContext->VSSetShader(m_pInstancingVS, nullptr, 0);
		pDeviceContext->PSSetShader(m_pShadowPS, nullptr, 0);
		pDeviceContext->IASetInputLayout(m_pShadowIL);*/

		auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		sm->BindPS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS);
		sm->BindVS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS);
	}

	// 頂点バッファをセット
	// インデックスバッファをセット
	DrawInstancing(aData, m_pVertexBuffer, m_pIndexBuffer, pTexture, mWorld);
}

//==========================================================
// 描画インスタンシング
// mesh使いまわし
//==========================================================
void CMesh::DrawInstancing(std::vector<DirectX::XMFLOAT4X4> aData, bool defaultShader,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	if (defaultShader)
	{
		// シェーダ設定
		auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		sm->BindPS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS);
		sm->BindVS(CMesh::SHADER_NAME_INSTANCING_MESH_PSVS);
	}

	std::vector<RENDER_DATA> setData;
	setData.resize(aData.size());

	for (auto cnt = 0; cnt < aData.size() ; ++cnt)
	{
		setData[cnt] = RENDER_DATA(aData[cnt], M_AMBIENT, M_DIFFUSE, M_SPECULAR, M_EMISSIVE);
	}

	// 頂点バッファをセット
	// インデックスバッファをセット
	DrawInstancing(setData, m_pVertexBuffer, m_pIndexBuffer, pTexture, mWorld);
}

void CMesh::DrawInstancing(std::vector<RENDER_DATA> aData, ID3D11Buffer* vertexB, ID3D11Buffer* indexB,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// シェーダ設定
	ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexB, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(indexB, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	if (pTexture)
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	SHADER_GLOBAL cb;
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;
	cb.mW = XMLoadFloat4x4(&m_mWorld);
	cb.mVP = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));

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
	sm->CBWrite(typeid(SHADER_GLOBAL).name(), &cb);
	sm->BindCB(typeid(SHADER_GLOBAL).name());

	SHADER_GLOBAL2 cb2;
	cb2.SetData(pCamera, dynamic_cast<CDirectionalLight*>(CLight::GetMain()), &m_material, pTexture);
	sm->CBWrite(typeid(SHADER_GLOBAL2).name(), &cb2);
	sm->BindCB(typeid(SHADER_GLOBAL2).name());

	// プリミティブ形状をセット
	const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	//--- データセット
	int cntNum = 0;

	//--- インスタンシング描画全て終わるまで
	while (cntNum < aData.size())
	{
		int cnt = 0;
		//--- 情報書き込み
		
		{
			INSTANCHING_DATA* imd = new INSTANCHING_DATA;
			INSTANCE_MATRIX* imtx = new INSTANCE_MATRIX;
			for (; cnt < MAX_WORLD_MATRIX; ++cnt, ++cntNum)
			{
				if (cnt >= aData.size() || cntNum >= aData.size())
					break;
				imtx->mWorld[cnt] = aData[cntNum].mWorld;
				imd->renderData[cnt] = aData[cntNum];
			}
			sm->CBWrite(CB_NAME_INSTANCE_MATERIAL, imd, sizeof(INSTANCHING_DATA));
			sm->CBWrite(CB_NAME_INSTANCE_MATRIX, imtx, sizeof(INSTANCE_MATRIX));
			delete imd;
			delete imtx;
		}
		sm->BindCB(CB_NAME_INSTANCE_MATERIAL);
		sm->BindCB(CB_NAME_INSTANCE_MATRIX);

		// ポリゴンの描画
		if (aData.size() > MAX_WORLD_MATRIX)
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(cnt), 0, 0, 0);
		else
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(aData.size()), 0, 0, 0);
	}
}

//==========================================================
// マテリアル設定
//==========================================================
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

