//=========================================================
// [shaderManager.cpp] 
// �쐬: 2022/06/27
//---------------------------------------------------------
// ssk�搶�Q�l
//=========================================================

//--- �x���}�~
#define _CRT_SECURE_NO_WARNINGS

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Shader/shaderStruct.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>
#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/negative.h>
#include <GraphicsSystem/PostProcess/outline.h>
#include <GraphicsSystem/PostProcess/monochrome.h>
#include <GraphicsSystem/PostProcess/darkness.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>

#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/errorMessage.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace MySpace::Graphics;

//--- �ÓI�ϐ�
static LPCWSTR g_pszCSODir = FORDER_DIR2(Data\\shader\\);
static std::string g_szCSODir = FORDER_DIR(Data/shader/);
const char* g_szShaderList[] = {
	//"Isoline",
	"PointToTriangle",
	//"Quad",
	"Triangle",
};

//inline bool loadBinaryFile(std::vector<char>* pOut, const char* filepath)
//{
//	std::ifstream file(filepath, std::ifstream::binary);
//	if (!file.good()) {
//		return false;
//	}
//	file.seekg(0, file.end);
//	size_t fileLength = file.tellg();
//	file.seekg(0, file.beg);
//
//	if (-1 == fileLength) {
//		return false;
//	}
//
//	pOut->resize(fileLength);
//	file.read(pOut->data(), fileLength);
//	return true;
//}

//==========================================================
// �R���X�g���N�^
//==========================================================
CShaderManager::CShaderManager()
	:m_fTessellationAmount(24.0f)
{
}

//==========================================================
// ������
//==========================================================
HRESULT CShaderManager::Init()
{
	HRESULT hr = S_OK;

	ConstantBufferSharedPtr cameraCB = std::make_shared<CConstantBuffer>();
	hr = cameraCB->MakeCPU(sizeof(SHADER_GLOBAL_CAMERA_LIGHT), Slot::CB_CAMERA_LIGHT, CConstantBuffer::EType::All);
	if(FAILED(hr))
	{
		return hr;
	}
	else
		this->SetCB(NAME_TO(SHADER_GLOBAL_CAMERA_LIGHT), cameraCB);

	{
		// Toon�p�ǂݍ���
		auto imgMgr = Application::Get()->GetSystem<CAssetsManager>()->GetImageManager();
		imgMgr->Load(FORDER_DIR(Data/Texture/ramp.png));
	}

	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szAssimpToon));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szAssimpToon, ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szMeshToon));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szMeshToon, ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szAssimoNega));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szAssimoNega, ps);
	}

	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szMeshNega));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szMeshNega, ps);
	}

	{
		ConstantBufferSharedPtr cb = std::make_shared<CConstantBuffer>();
		hr = cb->Make(sizeof(SHADER_RATE), Slot::CB_RATE, CConstantBuffer::EType::Pixel);
		if (FAILED(hr))
			return hr;
		else
			pSM->SetCB(NAME_TO(SHADER_RATE), cb);
	}

	//--- �萔�o�b�t�@
	// ���z
	ConstantBufferSharedPtr sunCB = std::make_shared<CConstantBuffer>();
	hr = sunCB->Make(sizeof(SHADER_SUN), Slot::CB_SUN_VP, CConstantBuffer::EType::Vertex);
	if (FAILED(hr))
		return hr;
	else
		pSM->SetCB(NAME_TO(SHADER_SUN), sunCB);

	// PS_DepthWrite
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szDepthWrite));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szDepthWrite, ps);
	}

	// VS_DepthWrite
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{	// �������ɂ̓Z�}���e�B�N�X���̐�����(�����)
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0,							D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	VertexShaderSharedPtr vs = std::make_shared<CVertexShader>();
	hr = vs->Make(CVertexName::GetCSO(CVertexName::szDepthWrite), layout, _countof(layout));
	if (FAILED(hr))
		return hr;
	else
		pSM->SetVS(CVertexName::szDepthWrite, vs);

	// PS_GBuffer
	{
		PixelShaderSharedPtr ps = std::make_shared<CPixelShader>();
		hr = ps->Make(CPixelName::GetCSO(CPixelName::szGBuffer));
		if (FAILED(hr))
			return hr;
		else
			pSM->SetPS(CPixelName::szGBuffer, ps);
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
	hr = CDarkness::InitShader();
	if (FAILED(hr))
		return hr;

	return hr;
}

//==========================================================
// �e�׽�ŉ������
//==========================================================
void CShaderManager::Uninit()
{
	for (auto & func : m_aCallbackFunc)
	{
		func.second = nullptr;
	}
	m_aCallbackFunc.clear();

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
// �X�V
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
// �֐��i�[
//==========================================================
void CShaderManager::AddFunction(std::string name, std::function<void(void)> func)
{
	m_aCallbackFunc[name] = func;
}

//==========================================================
// �֐��Ăяo�����A�o�C���h
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
// PS�o�C���h
//==========================================================
void CShaderManager::BindPS(std::string name, UINT slot)
{

#if BUILD_MODE
	if(!m_aPixelMap.count(name))
		Debug::CErrorMessage::DispMessage(name, "PixelShader No Data");
#endif // BUILD_MODE

	m_aPixelMap[name]->Bind(slot);
}

//==========================================================
// VS�o�C���h
//==========================================================
void CShaderManager::BindVS(std::string name, UINT slot)
{

#if BUILD_MODE
	if (!m_aVtxMap.count(name))
		Debug::CErrorMessage::DispMessage(name, "VertexShader No Data");
#endif // BUILD_MODE

	m_aVtxMap[name]->Bind(slot);
}

//==========================================================
// CB�o�C���h
//==========================================================
void CShaderManager::BindCB(std::string name, UINT slot)
{

#if BUILD_MODE
	if (!m_aConstantBufferMap.count(name))
		Debug::CErrorMessage::DispMessage(name, "ConstantBuffer No Data");
#endif // BUILD_MODE

	m_aConstantBufferMap[name]->Bind(slot);
}

//==========================================================
// MB�o�C���h
//==========================================================
void CShaderManager::BindMB(std::string name, UINT slot) 
{
	m_aMeshBuffMap[name]->Bind(slot);
}

//==========================================================
// �萔��������
//==========================================================
void CShaderManager::CBWrite(std::string name, void* data)
{
#if BUILD_MODE
	if (!m_aConstantBufferMap.count(name))
		Debug::CErrorMessage::DispMessage(name, "ConstantBuffer No Data");
#endif // BUILD_MODE

	m_aConstantBufferMap[name]->Write(data);
}

//==========================================================
// �萔��������
//==========================================================
void CShaderManager::CBWrite(std::string name, void* data, UINT size)
{
#if BUILD_MODE
	if (!m_aConstantBufferMap.count(name))
		Debug::CErrorMessage::DispMessage(name, "ConstantBuffer No Data");
#endif // BUILD_MODE

	m_aConstantBufferMap[name]->DynamicWrite(data, size);
}

//==========================================================
// CB�i�[ : ���O�ݒ�K�{
//==========================================================
void CShaderManager::SetCB(std::string name, ConstantBufferSharedPtr ptr)
{
	if (m_aConstantBufferMap.count(name))
		return;
	m_aConstantBufferMap.insert(ConstantPair(name, ptr)); 
}

//==========================================================
// VertexShader�i�[ : ���O�ݒ�K�{
//==========================================================
void CShaderManager::SetVS(std::string name, VertexShaderSharedPtr vs) 
{ 
	if (m_aVtxMap.count(name))
		return;
	m_aVtxMap.insert(VertexPair(name, vs)); 
}

//==========================================================
// PixelShader�i�[ : ���O�ݒ�K�{
//==========================================================
void CShaderManager::SetPS(std::string name, PixelShaderSharedPtr ps) 
{ 
	if (m_aPixelMap.count(name))
		return;
	m_aPixelMap.insert(PixelPair(name, ps)); 
}

//==========================================================
// MS�i�[ : ���O�ݒ�K�{
//==========================================================
void CShaderManager::SetMB(std::string name, MeshBufferSharedPtr mb) 
{ 
	if (m_aMeshBuffMap.count(name))
		return;
	m_aMeshBuffMap.insert(MeshBufferPair(name, mb));
}


#ifdef BUILD_MODE

//=========================================================
// vs�擾
//=========================================================
std::string CShaderManager::ImGuiGetVertexShader(std::string preview)
{
	static std::vector<std::string> cachePSName;
	// �擾�������ƊǗ�����size�ɈႢ������Ύ擾
	if (cachePSName.size() != m_aVtxMap.size())
	{
		cachePSName.clear();
		for (auto vertex : m_aVtxMap)
			cachePSName.push_back(vertex.first);
	}
	// GUI�\��
	Debug::SetTextAndAligned("VertexShader");
	return MySpace::Debug::DispComboSelect(cachePSName, "##VertexShader", preview);
}

//=========================================================
// ps�擾
//=========================================================
std::string CShaderManager::ImGuiGetPixelShader(std::string preview)
{
	static std::vector<std::string> cacheVSName;
	// �擾�������ƊǗ�����size�ɈႢ������Ύ擾
	if (cacheVSName.size() != m_aPixelMap.size())
	{
		cacheVSName.clear();
		for (auto pixel : m_aPixelMap)
			cacheVSName.push_back(pixel.first);
	}
	// GUI�\��
	Debug::SetTextAndAligned("PixelShader");
	return MySpace::Debug::DispComboSelect(cacheVSName, "##PixelShader", preview);
}

#endif // BUILD_MODE
