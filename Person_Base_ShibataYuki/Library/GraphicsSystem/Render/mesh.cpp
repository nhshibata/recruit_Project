//=========================================================
// [mesh.cpp] 
//---------------------------------------------------------
// �쐬:2022/06/27
// �X�V:2023/02/06 �V�F�[�_�[�����S�ɐؑ�
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Render/mesh.h>
#include <GraphicsSystem/Shader/shader.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <CoreSystem/Math/MyMath.h>

using namespace MySpace::Game;
using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- �}�N����`
#define M_DIFFUSE				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,0.3f)
#define M_SPECULAR				MySpace::MyMath::Vector4(0.1f,0.1f,0.1f,1.0f)
#define M_POWER					(0.0f)
#define M_AMBIENT				MySpace::MyMath::Vector4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE				MySpace::MyMath::Vector4(0.0f,0.0f,0.0f,1.0f)

//--- �\���̒�`
// �V�F�[�_�ɓn���l
//struct SHADER_GLOBAL 
//{
//	DirectX::XMMATRIX	mVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
//	DirectX::XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
//	DirectX::XMMATRIX	mTex;		// �e�N�X�`���s��(�]�u�s��)
//};

struct SHADER_GLOBAL2 
{
	XMVECTOR	vEye;		// ���_���W
	// ����
	XMVECTOR	vLightDir;	// ��������
	XMVECTOR	vLa;		// �����F(�A���r�G���g)
	XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	XMVECTOR	vLs;		// �����F(�X�y�L����)
	// �}�e���A��
	XMVECTOR	vAmbient;	// �A���r�G���g�F(+�e�N�X�`���L��)
	XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	XMVECTOR	vSpecular;	// �X�y�L�����F(+�X�y�L�������x)
	XMVECTOR	vEmissive;	// �G�~�b�V�u�F

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


//--- �ÓI�����o
ID3D11SamplerState* CMesh::m_pSamplerState;	// �e�N�X�`�� �T���v��

//==========================================================
// �R���X�g���N�^
//==========================================================
CMesh::CMesh()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CMesh::~CMesh()
{
}
//==========================================================
// �V�F�[�_������
//==========================================================
HRESULT CMesh::InitShader()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = Application::Get()->GetDevice();
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// �V�F�[�_������
	// inputLayout
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

#if 0
	hr = LoadShader("Vertex", "Pixel", &m_pVertexShader, &m_pInputLayout, &m_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) 
		return hr;
	hr = LoadShader("VS_Mesh", "PS_Mesh", &m_pInstancingVS, &m_pShadowIL, &m_pShadowPS, layout, _countof(layout));
	if (FAILED(hr)) 
		return hr;
#else
	//--- PS�AVS�̐����Ɠo�^
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(FORDER_DIR(Data/shader/Pixel.cso));
		if (FAILED(hr))
			return hr;
		else
			sm->SetPS(NAME_TO(Pixel), ps);

		VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
		hr = vs->Make(FORDER_DIR(Data/shader/Vertex.cso), layout, _countof(layout));
		if (FAILED(hr))
			return hr;
		else
			sm->SetVS(NAME_TO(Vertex), vs);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(FORDER_DIR(Data/shader/PS_Mesh.cso));
		if (FAILED(hr))
			return hr;
		else
			sm->SetPS(NAME_TO(PS_Mesh), ps);

		VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
		hr = vs->Make(FORDER_DIR(Data/shader/VS_Mesh.cso), layout, _countof(layout));
		if (FAILED(hr))
			return hr;
		else
			sm->SetVS(NAME_TO(VS_Mesh), vs);
	}

#endif // 0

	//--- �萔�o�b�t�@����
	//ConstantBufferSharedPtr cb_sg = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_sg0 = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_sg2 = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_sgm = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_im = std::make_shared<CConstantBuffer>();
	ConstantBufferSharedPtr cb_imtx = std::make_shared<CConstantBuffer>();
	//cb_sg->Make(sizeof(SHADER_GLOBAL), 0, CConstantBuffer::EType::Vertex);
	cb_sg0->MakeCPU(sizeof(SHADER_GLOBAL_WVP), 0, CConstantBuffer::EType::Vertex);
	cb_sg2->Make(sizeof(SHADER_GLOBAL2), 1, CConstantBuffer::EType::Pixel);
	cb_sgm->Make(sizeof(SHADER_MATERIAL), 2, CConstantBuffer::EType::Pixel);
	cb_imtx->MakeCPU(sizeof(INSTANCE_MATRIX), 4, CConstantBuffer::EType::Vertex);
	cb_im->MakeCPU(sizeof(INSTANCHING_MATERIAL), 5, CConstantBuffer::EType::Pixel);
	//sm->SetCB(typeid(SHADER_GLOBAL).name(), cb_sg);
	sm->SetCB(NAME_TO(SHADER_GLOBAL_WVP), cb_sg0);
	sm->SetCB(NAME_TO(SHADER_GLOBAL2), cb_sg2);
	sm->SetCB(NAME_TO(SHADER_MATERIAL), cb_sgm);
	sm->SetCB(NAME_TO(INSTANCE_MATRIX), cb_imtx);
	sm->SetCB(NAME_TO(INSTANCHING_MATERIAL), cb_im);

	//// �R���X�^���g�o�b�t�@ �C���X�^���V���O�p �쐬
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

	// �e�N�X�`�� �T���v������
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
// �V�F�[�_�I������
//==========================================================
void CMesh::FinShader()
{
	// �萔�o�b�t�@�̉��
	/*for (int i = 0; i < _countof(m_pConstantBuffer); ++i) {
		SAFE_RELEASE(m_pConstantBuffer[i]);
	}*/
	//SAFE_RELEASE(m_pConstantBufferI);
	
	// �s�N�Z���V�F�[�_���
	//SAFE_RELEASE(m_pPixelShader);
	//SAFE_RELEASE(m_pShadowPS);
	//// ���_�t�H�[�}�b�g���
	//SAFE_RELEASE(m_pInputLayout);
	//SAFE_RELEASE(m_pShadowIL);
	//// ���_�V�F�[�_���
	//SAFE_RELEASE(m_pVertexShader);
	//SAFE_RELEASE(m_pInstancingVS);

	SAFE_RELEASE(m_pSamplerState);
	
}

//==========================================================
// ������
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

	// �}�e���A���̏����ݒ�
	m_material.m_Diffuse = M_DIFFUSE;
	m_material.m_Ambient = M_AMBIENT;
	m_material.m_Specular = M_SPECULAR;
	m_material.m_Power = M_POWER;
	m_material.m_Emissive = M_EMISSIVE;

	return hr;
}

//==========================================================
// �I������
//==========================================================
void CMesh::Fin()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

//==========================================================
// �`��
//==========================================================
void CMesh::Draw(ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// �V�F�[�_�ݒ�
	ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();
	sm->BindPS(NAME_TO(Pixel));
	sm->BindVS(NAME_TO(Vertex));

	// ���_�o�b�t�@���Z�b�g
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	
	if(pTexture)
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	SHADER_GLOBAL_WVP cb;
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;
	cb.mW = XMLoadFloat4x4(&m_mWorld);
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetProjMatrix()));

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
	sm->CBWrite(NAME_TO(SHADER_GLOBAL_WVP), &cb, sizeof(SHADER_GLOBAL_WVP));
	sm->BindCB(NAME_TO(SHADER_GLOBAL_WVP));

	SHADER_GLOBAL2 cb2;
	cb2.SetData(pCamera, dynamic_cast<CDirectionalLight*>(CLight::GetMain()), &m_material, pTexture);
	/*pDeviceContext->UpdateSubresource(m_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);*/
	sm->CBWrite(NAME_TO(SHADER_GLOBAL2), &cb2);
	sm->BindCB(NAME_TO(SHADER_GLOBAL2));

	// �v���~�e�B�u�`����Z�b�g
	static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0�Ȃ�O�p�`�X�g���b�v
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	// �|���S���̕`��
	pDeviceContext->DrawIndexed(m_nNumIndex, 0, 0);
}

//==========================================================
// �`��C���X�^���V���O
//==========================================================
void CMesh::DrawInstancing(std::vector<RENDER_DATA> aData, bool defaultShader,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	if (defaultShader)
	{
		// �V�F�[�_�ݒ�
		/*ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();
		pDeviceContext->VSSetShader(m_pInstancingVS, nullptr, 0);
		pDeviceContext->PSSetShader(m_pShadowPS, nullptr, 0);
		pDeviceContext->IASetInputLayout(m_pShadowIL);*/

		auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		sm->BindPS(NAME_TO(PS_Mesh));
		sm->BindVS(NAME_TO(VS_Mesh));
	}

	// ���_�o�b�t�@���Z�b�g
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	DrawInstancing(aData, m_pVertexBuffer, m_pIndexBuffer, pTexture, mWorld);
}

//==========================================================
// �`��C���X�^���V���O
// mesh�g���܂킵
//==========================================================
void CMesh::DrawInstancing(std::vector<DirectX::XMFLOAT4X4> aData, bool defaultShader,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	if (defaultShader)
	{
		// �V�F�[�_�ݒ�
		auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
		sm->BindPS(NAME_TO(PS_Mesh));
		sm->BindVS(NAME_TO(VS_Mesh));
	}

	std::vector<RENDER_DATA> setData;
	setData.resize(aData.size());

	for (auto cnt = 0; cnt < aData.size() ; ++cnt)
	{
		setData[cnt] = RENDER_DATA(aData[cnt], M_AMBIENT, M_DIFFUSE, M_SPECULAR, M_EMISSIVE);
	}

	// ���_�o�b�t�@���Z�b�g
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	DrawInstancing(setData, m_pVertexBuffer, m_pIndexBuffer, pTexture, mWorld);
}

void CMesh::DrawInstancing(std::vector<RENDER_DATA> aData, ID3D11Buffer* vertexB, ID3D11Buffer* indexB,
						   ID3D11ShaderResourceView* pTexture, XMFLOAT4X4* mWorld)
{
	auto sm = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// �V�F�[�_�ݒ�
	ID3D11DeviceContext* pDeviceContext = Application::Get()->GetDeviceContext();

	// ���_�o�b�t�@���Z�b�g
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexB, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	pDeviceContext->IASetIndexBuffer(indexB, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	if (pTexture)
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	//--- b0
	SHADER_GLOBAL_WVP cb;
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;
	cb.mW = XMLoadFloat4x4(&m_mWorld);
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetProjMatrix()));

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
	sm->CBWrite(NAME_TO(SHADER_GLOBAL_WVP), &cb, sizeof(SHADER_GLOBAL_WVP));
	sm->BindCB(NAME_TO(SHADER_GLOBAL_WVP));

	//--- b1
	SHADER_GLOBAL_CAMERA_LIGHT sg;
	sg.vEye = XMLoadFloat3(&CCamera::GetMain()->Transform()->GetPos());
	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::GetMain());
	sg.vLightDir = XMLoadFloat3(&pLight->GetDir());
	sg.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	sg.vLa = XMLoadFloat4(&pLight->GetAmbient());
	sg.vLs = XMLoadFloat4(&pLight->GetSpecular());

	sm->CBWrite(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), &sg, sizeof(SHADER_GLOBAL_CAMERA_LIGHT));
	sm->BindCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT));

	//--- b2
	SHADER_MATERIAL cb2;
	cb2.SetData(m_material.m_Diffuse, m_material.m_Ambient, m_material.m_Specular, m_material.m_Emissive, DirectX::XMFLOAT4(0, 0, 0, 0), m_material.m_Power, pTexture != nullptr);
	sm->CBWrite(NAME_TO(SHADER_MATERIAL), &cb2);
	sm->BindCB(NAME_TO(SHADER_MATERIAL));

	// �v���~�e�B�u�`����Z�b�g
	const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0�Ȃ�O�p�`�X�g���b�v
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[0]);

	//--- �f�[�^�Z�b�g
	int cntNum = 0;

	//--- �C���X�^���V���O�`��S�ďI���܂�
	while (cntNum < aData.size())
	{
		int cnt = 0;
		//--- ��񏑂����� b4,5
		//{
			INSTANCE_MATRIX* imtx = new INSTANCE_MATRIX;
			INSTANCHING_MATERIAL* imd = new INSTANCHING_MATERIAL;
			for (; cnt < MAX_WORLD_MATRIX; ++cnt, ++cntNum)
			{
				if (cnt >= aData.size() || cntNum >= aData.size())
					break;
				imtx->mWorld[cnt] = aData[cntNum].mWorld;
				imd->renderData[cnt] = aData[cntNum];
			}
			sm->CBWrite(NAME_TO(INSTANCE_MATRIX), imtx, sizeof(INSTANCE_MATRIX));
			sm->CBWrite(NAME_TO(INSTANCHING_MATERIAL), imd, sizeof(INSTANCHING_MATERIAL));
			sm->BindCB(NAME_TO(INSTANCE_MATRIX));
			sm->BindCB(NAME_TO(INSTANCHING_MATERIAL));
			
		//}

		// �|���S���̕`��
		if (aData.size() > MAX_WORLD_MATRIX)
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(cnt), 0, 0, 0);
		else
			pDeviceContext->DrawIndexedInstanced(static_cast<UINT>(m_nNumIndex), static_cast<UINT>(aData.size()), 0, 0, 0);

		delete imd;
		delete imtx;
	}
}

//==========================================================
// �}�e���A���ݒ�
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

