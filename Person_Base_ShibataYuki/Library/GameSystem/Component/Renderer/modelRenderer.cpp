//=========================================================
// [modelRenderer.h] 
// �쐬: 2022/06/27
// �X�V: 2022/07/04 ����
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Component/Light/directionalLight.h>

#include <GraphicsSystem/DirectX/DXDevice.h>
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>


using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;


CModelRenderer::CModelRenderer(std::shared_ptr<CGameObject> owner)
	:CMeshRenderer(owner),m_modelName(CHARACTER_PATH(mukade_head.obj))
{

}
CModelRenderer::~CModelRenderer()
{
}
void CModelRenderer::SetModel(std::string name)
{
	// �|�C���^���󂯎��
	if (m_pModel = CModelManager::Get().GetModel(name); m_pModel.lock())
	{
		m_modelName = name;

		// ���f�����a�̐ݒ�
		auto size = Transform()->GetScale();
		float scale = size.x;
		if (scale < size.y || scale < size.z)
		{
			scale = size.y;
			if (scale < size.z)
				scale = size.z;
		}
		SetBSRadius(m_pModel.lock()->GetRadius() * scale);
	}
}
void CModelRenderer::Awake()
{
	GetOwner()->SetLayer(CLayer::E_Layer::MODEL);
	SetModel(m_modelName);
}
void CModelRenderer::Init()
{
	//SetModel(m_modelName);
	CMeshRenderer::Init();
}
void CModelRenderer::Update()
{
}
bool CModelRenderer::Draw()
{
	if (!CMeshRenderer::Draw())return false;
	if (!m_pModel.lock())return false;

	// �s�����`��
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	CLight* pLight = CLight::Get();
	pLight->SetDisable(GetLightEnable());	// ���C�e�B���O����
	
	m_pModel.lock()->Draw(CDXDevice::Get().GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);

	pLight->SetEnable();	// ���C�e�B���O�L��

	// �����������`��
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	m_pModel.lock()->Draw(CDXDevice::Get().GetDeviceContext(), mtx, EByOpacity::eTransparentOnly);

	CDXDevice::Get().SetZBuffer(true);			// ���u�����f�B���O����
	CDXDevice::Get().SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// �����L��

	return true;
}
bool CModelRenderer::Draw(int no)
{
	XMFLOAT4X4 mtx = Transform()->GetWorldMatrix();
	
	if (!m_pModel.lock())return false;
	m_pModel.lock()->Draw(CDXDevice::Get().GetDeviceContext(), mtx, EByOpacity::eOpacityOnly);
	m_pModel.lock()->Draw(CDXDevice::Get().GetDeviceContext(), mtx);

	return true;
}
#ifdef BUILD_MODE


void CModelRenderer::ImGuiDebug()
{
	using namespace MySpace::Debug;
	static std::vector<std::string> s_XModelList;
	static std::vector<std::string> s_ObjModelList;
	static std::vector<std::string> s_FbxModelList;

	if (s_XModelList.empty() && s_ObjModelList.empty() && s_FbxModelList.empty() || ImGui::Button("���f�� reload"))
	{
		MySpace::System::CFilePath file;
		s_XModelList = file.GetAllFileName(MODEL_PATH, ".x");
		s_ObjModelList = file.GetAllFileName(MODEL_PATH, ".obj");
		s_FbxModelList = file.GetAllFileName(MODEL_PATH, ".fbx");
	}

	// ���O����
	m_modelName = InputString(m_modelName, "�ݒ胂�f��");

	// ���f��
	if (auto name = DispMenuBar(s_XModelList, "xFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get().GetModel(name);
	}
	
	if (auto name = DispMenuBar(s_ObjModelList, "objFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get().GetModel(name);
	}
	
	if (auto name = DispMenuBar(s_FbxModelList, "fbxFile"); !name.empty())
	{
		m_modelName = name;

		// �|�C���^���󂯎��
		m_pModel = CModelManager::Get().GetModel(name);
	}
}

#endif // BUILD_MODE