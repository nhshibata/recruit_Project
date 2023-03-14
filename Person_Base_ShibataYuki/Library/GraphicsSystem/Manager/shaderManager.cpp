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
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>
#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/negative.h>
#include <GraphicsSystem/PostProcess/monochrome.h>
#include <GraphicsSystem/PostProcess/outline.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>

#include <DebugSystem/imGuiPackage.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

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

	ConstantBufferSharedPtr cameraCB = std::make_shared<CConstantBuffer>();
	hr = cameraCB->MakeCPU(sizeof(SHADER_GLOBAL_CAMERA_LIGHT), 1, CConstantBuffer::EType::All);
	if(FAILED(hr))
	{
		return hr;
	}
	else
		this->SetCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), cameraCB);

	{
		// Toon用読み込み
		auto imgMgr = Application::Get()->GetSystem<CAssetsManager>()->GetImageManager();
		imgMgr->Load(FORDER_DIR(Data/Texture/ramp.png));
	}

	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_AssimpToon.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_AssimpToon", ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_MeshToon.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_MeshToon", ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_AssimpNega.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_AssimpNega", ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_MeshNega.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_MeshNega", ps);
	}

	{
		ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
		hr = cb->Make(sizeof(SHADER_RATE), 7, CConstantBuffer::EType::Pixel);
		if (FAILED(hr))
			return hr;
		else
			pSM->SetCB(NAME_TO(SHADER_RATE), cb);
	}

	//--- 定数バッファ
	// 太陽
	ConstantBufferSharedPtr sunCB = std::make_shared<CConstantBuffer>();
	hr = sunCB->Make(sizeof(SHADER_SUN), 6, CConstantBuffer::EType::Vertex);
	if (FAILED(hr))
		return hr;
	else
		pSM->SetCB(NAME_TO(SHADER_SUN), sunCB);

	// PS_DepthWrite
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_DepthWrite.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_DepthWrite", ps);
	}

	// VS_DepthWrite
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{	// 第二引数にはセマンティクス名の数字を(あれば)
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0,							D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	hr = vs->Make(CSO_PATH(VS_DepthWrite.cso), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	else
		pSM->SetVS("VS_DepthWrite", vs);

	// PS_GBuffer
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CSO_PATH(PS_GBuffer.cso));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS("PS_GBuffer", ps);
	}

	hr = CGaussianBlur::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CBloom::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CNegative::InitShader();
	if (FAILED(hr))
		return hr;
	hr = CMonochrome::InitShader();
	if (FAILED(hr))
		return hr;
	hr = COutline::InitShader();
	if (FAILED(hr))
		return hr;

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
	/*if (!m_aConstantBufferMap.count(name))
		return;*/
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
