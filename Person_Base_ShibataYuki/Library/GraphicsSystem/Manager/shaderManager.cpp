//=========================================================
// [shaderManager.cpp] 
// 作成: 2022/06/27
//---------------------------------------------------------
// ssk先生参考
//=========================================================

//--- 警告抑止
#define _CRT_SECURE_NO_WARNINGS

//--- インクルード部
#include <Application/Application.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <fstream>
#include <DebugSystem/imGuiPackage.h>


using namespace MySpace::Graphics;

//--- 静的変数
static LPCWSTR g_pszCSODir = FORDER_DIR2(Data\\shader\\);
static std::string g_szCSODir = FORDER_DIR(Data/shader/);
const char* g_szShaderList[] = {
	//"Isoline",
	"PointToTriangle",
	//"Quad",
	"Triangle",
};

inline bool loadBinaryFile(std::vector<char>* pOut, const char* filepath)
{
	std::ifstream file(filepath, std::ifstream::binary);
	if (!file.good()) {
		return false;
	}
	file.seekg(0, file.end);
	size_t fileLength = file.tellg();
	file.seekg(0, file.beg);

	if (-1 == fileLength) {
		return false;
	}

	pOut->resize(fileLength);
	file.read(pOut->data(), fileLength);
	return true;
}

//==========================================================
// コンストラクタ
//==========================================================
CShaderManager::CShaderManager()
	:m_fTessellationAmount(24.0f)
{
}

//==========================================================
// 初期化
//==========================================================
HRESULT CShaderManager::Init()
{
	HRESULT hr = S_OK;
	//auto device = Application::Get()->GetDevice();

	//for (int cnt = 0 ; cnt < static_cast<int>(EShaderType::MAX); ++cnt)
	//{
	//	/*ID3D10Blob* hullShaderBuffer;
	//	ID3D10Blob* domainShaderBuffer;
	//	ID3D10Blob* errorMessage;*/
	//	std::string hullName = "HS"; 
	//	std::string domainName = "DS";
	//	hullName += g_szShaderList[cnt];
	//	domainName += g_szShaderList[cnt];

	//	// シェーダー読み込み
	//	//hr = D3DX11CompileFromFile(std::string(g_szCSODir + hullName + ".cso").c_str(), NULL, NULL, hullName.c_str(), "hs_5_0", //D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
	//	//	&hullShaderBuffer, &errorMessage, NULL);
	//	//if (FAILED(hr)) { return E_FAIL; }
	//	//hr = D3DX11CompileFromFile(std::string(g_szCSODir + domainName + ".cso").c_str(), NULL, NULL, domainName.c_str(), "ds_5_0", //D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
	//	//	&domainShaderBuffer, &errorMessage, NULL);

	//	std::vector<char> byteCode; /*pOutByteCode ? pOutByteCode :*/
	//	auto* pTarget = &byteCode;
	//	if (!loadBinaryFile(pTarget, std::string(g_szCSODir + hullName + ".cso").c_str())) {
	//		throw std::runtime_error("の読み込みに失敗");
	//	}
	//	
	//	std::vector<char> byteCode2; /*pOutByteCode ? pOutByteCode :*/
	//	auto* pTarget2 = &byteCode2;
	//	if (!loadBinaryFile(pTarget2, std::string(g_szCSODir + domainName + ".cso").c_str())) {
	//		throw std::runtime_error("の読み込みに失敗");
	//	}

	//	HullShaderSharedPtr pHullShader = std::make_shared<CHullShader>();
	//	DomainShaderSharedPtr pDomainShader = std::make_shared<CDomainShader>();
	//	pHullShader->Make(pTarget->data(), static_cast<size_t>(pTarget->size()));
	//	pDomainShader->Make(pTarget2->data(), static_cast<size_t>(pTarget2->size()));
	//	// 格納
	//	//SetTessellation(g_szShaderList[cnt], pHullShader, pDomainShader);
	//}
	//
	//MatrixBufferType param;
	//TessellationBufferType param2;
	//D3D11_SUBRESOURCE_DATA initData = {};
	//initData.pSysMem = &param;
	//initData.SysMemPitch = sizeof(param);
	//D3D11_SUBRESOURCE_DATA initData2 = {};
	//initData2.pSysMem = &param2;
	//initData2.SysMemPitch = sizeof(param2);

	//CConstantBuffer::Ptr cb = std::make_shared<CConstantBuffer>();
	//CConstantBuffer::Ptr cb2 = std::make_shared<CConstantBuffer>();
	//cb->Make(sizeof(MatrixBufferType),0, CConstantBuffer::EType::MAX, &initData);
	//cb2->Make(sizeof(TessellationBufferType),0, CConstantBuffer::EType::MAX, &initData2);
	//SetCB("MatrixBufferType", cb);
	//SetCB("TessellationBufferType", cb2);

	ConstantBufferSharedPtr sunCB = std::make_shared<CConstantBuffer>();
	hr = sunCB->MakeCPU(sizeof(SHADER_GLOBAL_CAMERA_LIGHT), 1, CConstantBuffer::EType::All);
	if(FAILED(hr))
	{
		return hr;
	}
	else
		this->SetCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), sunCB);

	return hr;
}

//==========================================================
// 各ｸﾗｽで解放処理
//==========================================================
void CShaderManager::Uninit()
{
	/*for (auto & func : m_aCallbackFunc)
	{
		func.second = nullptr;
	}
	m_aCallbackFunc.clear();*/

	for (auto & pixel : m_aPixelMap)
	{
		pixel.second.reset();
	}
	
	for (auto & vtx : m_aVtxMap)
	{
		vtx.second.reset();
	}

	for (auto & con : m_aConstantBufferMap)
	{
		con.second.reset();
	}

	for (auto & hs : m_aHullMap)
	{
		hs.second.reset();
	}
	
	for (auto & ds : m_aDomainMap)
	{
		ds.second.reset();
	}
}

//==========================================================
// 更新
//==========================================================
void CShaderManager::Update()
{
	SHADER_GLOBAL_CAMERA_LIGHT sg;
	auto pCamera = CCamera::GetMain();
	sg.vEye = XMLoadFloat3(&pCamera->Transform()->GetPos());
	CDirectionalLight* pLight = dynamic_cast<CDirectionalLight*>(CLight::GetMain());
	if (!pLight)
		return;

	sg.vLightDir = XMLoadFloat3(&pLight->GetDir());
	sg.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	sg.vLa = XMLoadFloat4(&pLight->GetAmbient());
	sg.vLs = XMLoadFloat4(&pLight->GetSpecular());

	CBWrite(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), &sg, sizeof(SHADER_GLOBAL_CAMERA_LIGHT));
	BindCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT));
}

//==========================================================
// 関数格納
//==========================================================
void CShaderManager::AddFunction(std::string name, std::function<void(void)> func)
{
	m_aCallbackFunc[name] = func;
}

//==========================================================
// 関数呼び出しかつ、バインド
//==========================================================
void CShaderManager::CallBackFuncAndBind(std::string ps, std::string vs)
{
	if (m_aCallbackFunc.count(ps))
	{
		m_aCallbackFunc[ps]();
	}
	if (m_aCallbackFunc.count(vs))
	{
		m_aCallbackFunc[vs]();
	}
	
	if(!ps.empty())
		BindPS(ps);
	if(!vs.empty())
		BindVS(vs);
}

//==========================================================
// PSバインド
//==========================================================
void CShaderManager::BindPS(std::string name, UINT slot)
{
	/*if (!m_aPixelMap.count(name))
		return;*/
	m_aPixelMap[name]->Bind(slot);
}

//==========================================================
// VSバインド
//==========================================================
void CShaderManager::BindVS(std::string name, UINT slot)
{
	if (!m_aVtxMap.count(name))
		return;
	m_aVtxMap[name]->Bind(slot);
}

//==========================================================
// CBバインド
//==========================================================
void CShaderManager::BindCB(std::string name, UINT slot)
{
	if (!m_aConstantBufferMap.count(name))
		return;
	m_aConstantBufferMap[name]->Bind(slot);
}

//==========================================================
// MBバインド
//==========================================================
void CShaderManager::BindMB(std::string name, UINT slot) 
{
	if (!Find<MeshBufferSharedPtr>(name, m_aMeshBuffMap))
		return;
	m_aMeshBuffMap[name]->Bind(slot);
}

//==========================================================
// 定数書き込み
//==========================================================
void CShaderManager::CBWrite(std::string name, void* data)
{
	m_aConstantBufferMap[name]->Write(data);
}

//==========================================================
// 定数書き込み
//==========================================================
void CShaderManager::CBWrite(std::string name, void* data, UINT size)
{
	if (!m_aConstantBufferMap.count(name))
		return;
	m_aConstantBufferMap[name]->DynamicWrite(data, size);
}

//==========================================================
// CB格納 : 名前設定必須
//==========================================================
void CShaderManager::SetCB(std::string name, ConstantBufferSharedPtr ptr)
{
	if (m_aConstantBufferMap.count(name))
		return;
	m_aConstantBufferMap.insert(ConstantPair(name, ptr)); 
}

//==========================================================
// VertexShader格納 : 名前設定必須
//==========================================================
void CShaderManager::SetVS(std::string name, VertexShaderSharedPtr vs) 
{ 
	if (m_aVtxMap.count(name))
		return;
	m_aVtxMap.insert(VertexPair(name, vs)); 
}

//==========================================================
// PixelShader格納 : 名前設定必須
//==========================================================
void CShaderManager::SetPS(std::string name, PixelShaderSharedPtr ps) 
{ 
	if (m_aPixelMap.count(name))
		return;
	m_aPixelMap.insert(PixelPair(name, ps)); 
}

//==========================================================
// MS格納 : 名前設定必須
//==========================================================
void CShaderManager::SetMB(std::string name, MeshBufferSharedPtr mb) 
{ 
	if (m_aMeshBuffMap.count(name))
		return;
	m_aMeshBuffMap.insert(MeshBufferPair(name, mb));
}

//==========================================================
// シェーダ設定
//==========================================================
bool CShaderManager::SetShaderParameters(
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	float tessellationAmount)
{
	HRESULT hr = S_OK;
	ID3D11DeviceContext * pDC = Application::Get()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	TessellationBufferType* dataPtr2;
	auto buffer = m_aConstantBufferMap["MatrixBufferType"]->GetBuffer();
	auto buffer2 = m_aConstantBufferMap["TessellationBufferType"]->GetBuffer();

	// Transpose the matrices to prepare them for the shader.
	XMMatrixTranspose(worldMatrix);
	XMMatrixTranspose(viewMatrix);
	XMMatrixTranspose(projectionMatrix);

	// Lock the matrix constant buffer so it can be written to.
	hr = pDC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))return false;
	// Get a pointer to the data in the matrix constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	pDC->Unmap(buffer, 0);

	// Set the position of the matrix constant buffer in the domain shader.
	//bufferNumber = 0;

	// Finally set the matrix constant buffer in the domain shader with the updated values.
	pDC->DSSetConstantBuffers(0, 1, &buffer);

	// Lock the tessellation constant buffer so it can be written to.
	hr = pDC->Map(buffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))return false;

	// Get a pointer to the data in the tessellation constant buffer.
	dataPtr2 = (TessellationBufferType*)mappedResource.pData;
	// Copy the tessellation data into the constant buffer.
	dataPtr2->tessellationAmount = tessellationAmount;
	dataPtr2->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	pDC->Unmap(buffer2, 0);

	// Set the position of the tessellation constant buffer in the hull shader.
	//bufferNumber = 0;

	// Now set the tessellation constant buffer in the hull shader with the updated values.
	pDC->HSSetConstantBuffers(0, 1, &buffer2);

	return true;
}

//==========================================================
// 描画
//==========================================================
void CShaderManager::Render(EShaderType etype, std::string cb, std::string vs, std::string ps, std::string mb)
{
	//auto pCamera = Game::CCamera::GetMain();

	////MatrixBufferType data1 = { pCamera->GetWorldMatrix(0), pCamera->GetLookAtMatrix(), pCamera->GetProjectionMatrix() };
	////m_ConstantMap["MatrixBufferType"]->Write(&data1);

	////TessellationBufferType dataPtr2 = {m_fTessellationAmount ,XMFLOAT3(0.0f, 0.0f, 0.0f) };
	////m_ConstantMap["TessellationBufferType"]->Write(&dataPtr2);
	////m_ConstantMap["TessellationBufferType"]->Bind();

	//BindCB(cb);

	//if (1/*||GetAsyncKeyState(VK_HOME)*/) {
	////	m_pHullMap[g_szShaderList[static_cast<int>(etype)]]->Bind();
	////	m_pDomainMap[g_szShaderList[static_cast<int>(etype)]]->Bind();
	//}

	//BindVS(vs);

	//SetShaderParameters(pCamera->GetWorldMatrix(0), pCamera->GetLookAtMatrix(), pCamera->GetProjectionMatrix(), m_fTessellationAmount);

	//BindPS(ps);
	//
	//BindMB(mb);
}

//==========================================================
// 描画後処理
//==========================================================
void CShaderManager::EndRender()
{
	ID3D11DeviceContext* pDC = Application::Get()->GetDeviceContext();
	pDC->VSSetConstantBuffers(0, 1, nullptr);
	//pDC->VSSetShader(nullptr, nullptr, 0);
	//pDC->IASetInputLayout(nullptr);
	//pDC->HSSetShader(nullptr, nullptr, 0);
	//pDC->DSSetShader(nullptr, NULL, 0);
	//pDC->PSSetShader(nullptr, nullptr, 0);
}


#ifdef BUILD_MODE

//=========================================================
// vs取得
//=========================================================
std::string CShaderManager::ImGuiGetVertexShader(std::string preview)
{
	static std::vector<std::string> ver;
	if (ver.size() != m_aVtxMap.size())
	{
		for (auto vertex : m_aVtxMap)
			ver.push_back(vertex.first);
	}
	return MySpace::Debug::DispComboSelect(ver, "VertexShader", preview);
}

//=========================================================
// ps取得
//=========================================================
std::string CShaderManager::ImGuiGetPixelShader(std::string preview)
{
	static std::vector<std::string> ver;
	if (ver.size() != m_aPixelMap.size())
	{
		for (auto pixel : m_aPixelMap)
			ver.push_back(pixel.first);
	}
	return MySpace::Debug::DispComboSelect(ver, "PixelShader", preview);
}

#endif // BUILD_MODE
