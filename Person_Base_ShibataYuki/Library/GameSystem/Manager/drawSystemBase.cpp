//=========================================================
// [drawSystem.cpp]
// �h���N���X
//---------------------------------------------------------
// 
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystemBase.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Shader/shaderAssets.h>

#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CDrawSystemBase::CDrawSystemBase()
	:m_bIsSortNecessary(true)
{
	m_aPolygonList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();
	m_pDepthShadow = std::make_unique<CDepthShadow>();

#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
	m_bFrustum = true;
	m_bShadowView = true;
#endif // BUILD_MODE

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDrawSystemBase::~CDrawSystemBase()
{
	m_pDepthShadow.reset();

	CShaderAssets::Uninit();
}

//==========================================================
// ������
//==========================================================
void CDrawSystemBase::Init()
{
	m_pDepthShadow->InitShader();

	CShaderAssets::Init();
}

//==========================================================
// �o�^
//==========================================================
int CDrawSystemBase::PolygonRegist(std::weak_ptr<CPolygonRenderer> render)
{
	int ret = CMapSystemBase::RegistToSystem(render.lock());
	// �|���S�����Ɋi�[
	m_aPolygonList.push_back(render);
	return ret;
}

//==========================================================
// �o�^ 
// �C���X�^���V���O�`��̂��߂ɏ����i�[����
//==========================================================
void CDrawSystemBase::SetInstanchingModel(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data)
{
	std::string pushName = name + "#" + ps + "#" + vs;
	m_aInstancingModelMap[pushName].push_back(data);
}

//==========================================================
// �o�^
//==========================================================
void CDrawSystemBase::SetInstanchingMesh(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data, CMesh* mesh)
{
	std::string pushName = name + "#" + ps + "#" + vs;
	if (!m_aInstancingMeshMap.count(pushName))
		m_aInstancingMeshMap[pushName].pMesh = mesh;

	m_aInstancingMeshMap[pushName].aData.push_back(data);
}

//==========================================================
// ���X�g�擾
//==========================================================
std::vector<std::weak_ptr<CRenderer>> CDrawSystemBase::GetList()
{
	std::vector<std::weak_ptr<CRenderer>> ret;
	for (auto & it : m_aIntMap)
	{
		ret.push_back(it.second);
	}
	return ret;
}

//=========================================================
// map�i�[���̖��O�𕪊����A�ԋp
// 0:�o�^�� 1:PS 2:VS
// FIXME:�����_���ɕς��Ă���������
//=========================================================
const std::vector<std::string> CDrawSystemBase::GetPSVSName(const std::string name)
{
	std::vector<std::string> ret;
	// ps���W�擾
	auto startIndex = name.find("#");
	// ���O�i�[
	ret.push_back(name.substr(0, startIndex));
	startIndex += 1;
	// vs���W�擾
	auto endIndex = name.find("#", startIndex);
	//--- �����؂�o���i�[
	ret.push_back(name.substr(startIndex, endIndex - startIndex));
	endIndex += 1;
	ret.push_back(name.substr(endIndex, name.size() - (startIndex + endIndex)));
	return ret;
}

//==========================================================
// ���O
//==========================================================
std::weak_ptr<CRenderer> CDrawSystemBase::ExecutSystem(int idx)
{
	auto release = IDToData(idx);

	// �|���S���Ȃ�j���������߰��Ă�T���A���񃊃X�g���珜�O
	if (auto polygon = std::dynamic_pointer_cast<CPolygonRenderer>(release.lock()); polygon)
	{
		for (auto it = m_aPolygonList.begin(); it != m_aPolygonList.end(); ++it)
		{
			if (release.lock() == (*it).lock())
			{
				m_aPolygonList.erase(it);
				break;
			}
		}
	}
	return CMapSystemBase::ExecutSystem(idx);
}

//==========================================================
// �X�V
//==========================================================
void CDrawSystemBase::Update()
{
	// CRenderer::Draw��bool��Ԃ�
	// true�ł���Ε`�悷��
	// �`�搔�Ȃǂ̊m�F������Ȃ�΂�������p����

	//--- ���C���[�ŕ��ёւ�
	// �ǉ����ꂽ�Ƃ��Ȃ�
	if (m_bIsSortNecessary)
	{
		Sort();
		m_bIsSortNecessary = false;
	}

	//--- BG�̕`��
	int UIIdx = 0;	// UI�����ԖڋL�����邽�߂̕ϐ�
	for (int cnt = 0; cnt < m_aPolygonList.size(); ++cnt, ++UIIdx)
	{
		// �|�C���^�m�F
		if (!m_aPolygonList[cnt].lock())
			continue;

		if (m_aPolygonList[cnt].lock()->GetZ() > static_cast<int>(CPolygonRenderer::EZValue::BG))
			break;

		// �`��\�ȏ�Ԃ��m�F
		if (!m_aPolygonList[cnt].lock()->IsActive() || !m_aPolygonList[cnt].lock()->IsVisible())
			continue;

		//--- �`��
		m_aPolygonList[cnt].lock()->Draw();
	}


	//--- 3D�`��(Mesh,Model)
	for (auto & it : m_aIntMap)
	{
		auto render = it.second;

		// �|�C���^�m�F
		if (!render.lock())
			continue;

		// �`��\�ȏ�Ԃ��m�F
		if (!render.lock()->IsActive() || !render.lock()->IsVisible())
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render.lock()->GetName();
#endif // _DEBUG

		//--- Mesh���߰���(����ьp��)���m�F
		// �J�����O�t���O���m�F
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh)
		{

			float fRadius = mesh->GetBSRadius();
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;

			//--- ��ׂɉf�邩����
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius())==CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42, mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
#ifdef BUILD_MODE
				++m_nSkipCnt;
				if (m_bFrustum)
					continue;
#else
				continue;
#endif // _DEBUG
			}

		}

		// �����܂ł�����`��
		render.lock()->Draw();
	}

	//--- 3D�`��
	// �C���X�^���V���O�����̂Ŋ֐���
	Draw3D();		// 3D���`��

	// �N���A
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();

	//--- UI�����`��
	for (int cnt = UIIdx; cnt < m_aPolygonList.size(); ++cnt)
	{
		// �|�C���^�m�F
		if (!m_aPolygonList[cnt].lock())
			continue;

		// �`��\�ȏ�Ԃ��m�F
		if (!m_aPolygonList[cnt].lock()->IsActive() || !m_aPolygonList[cnt].lock()->IsVisible())
			continue;

		//--- �`��
		m_aPolygonList[cnt].lock()->Draw();
	}

}

//==========================================================
// ����
//==========================================================
void CDrawSystemBase::Sort()
{
	//--- ����K�v�m�F
	if (m_aPolygonList.size() <= 1)
		return;

	//--- �����_���Ő���(����)
	std::sort(m_aPolygonList.begin(), m_aPolygonList.end(), [](auto const& s1, auto const& s2)->bool
	{
		return s1.lock()->GetZ() < s2.lock()->GetZ();
	});
}

//==========================================================
// 3D�C���X�^���V���O�e�`��
// TODO:�e�`��t���O�ŉe��`�悷�邩���߂�d�g�݂��Ȃ�
//==========================================================
void CDrawSystemBase::Draw3DShadow()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	//--- �[�x�l�������ݏ���
	m_pDepthShadow->Begin();

	//--- �o�^���ꂽ���f�����ʂɕ`��
	for (auto & instancingModel : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(instancingModel.first);
		if (aName.size() != 3)
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);
		// ���f�����������Ă��Ȃ����ꉞ�m�F
		if (!model)
			continue;

		model->DrawInstancing(pDX->GetDeviceContext(), instancingModel.second, EByOpacity::eOpacityOnly, false);
	}

	//--- Mesh
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.aData.size() == 0)// �ꉞ�m�F
			continue;

		// �r���{�[�h���m�F
		if (CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second.pMesh); bill != nullptr)
		{
			auto aName = GetPSVSName(mesh.first);
			if (aName.size() != 3)
				continue;

			// ø����ݒ�
			auto image = pAssets->GetImageManager()->GetResource(aName[0]);
			auto tex = image ? image->GetSRV() : NULL;
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false);
		}
	}

	//--- �[�x�l�������ݏI��
	m_pDepthShadow->End();

	//--- �[�x�lø����ݒ�
	// t4
	m_pDepthShadow->SetUpTexture(4);
}

//==========================================================
// 3D�C���X�^���V���O�`��
//==========================================================
void CDrawSystemBase::Draw3D()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();

	// �e�`��
	Draw3DShadow();	

	pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	CLight* pLight = CLight::GetMain();
	//pLight->SetDisable();			// ���C�e�B���O����
	//pDX->SetZBuffer(true);		// Z��������

	//--- �o�^���ꂽ���f�����ʂɕ`��
	//--- �s�����`��
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(intancingModel.first);
		if (aName.size() != 3)
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);
		// ���f�����������Ă��Ȃ����ꉞ�m�F
		if (!model)
			continue;
		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	pLight->SetEnable();			// ���C�e�B���O�L��


	//--- �����������`��
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);			// Z��������

	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = GetPSVSName(intancingModel.first);
		if (aName.size() != 3)
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(aName[0]);

		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eTransparentOnly, false);
	}

	pDX->SetZWrite(true);			// Z��������
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	//--- ���b�V���C���X�^���V���O�`��
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.aData.size() == 0)// �ꉞ�m�F
			continue;

		std::vector<std::string> aName = GetPSVSName(mesh.first);
		if (aName.size() != 3)
			continue;

		// shaderBind
		pSM->CallBackFuncAndBind(aName[1], aName[2]);

		// �r���{�[�h���m�F
		if (CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second.pMesh); bill != nullptr)
		{
			// ø����ݒ�
			auto image = pAssets->GetImageManager()->GetResource(aName[0]);
			auto tex = image ? image->GetSRV() : NULL;
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			mesh.second.pMesh->DrawInstancing(mesh.second.aData, false);
		}
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}

	//--- �ݒ��߂�
	pLight->SetEnable();		// ���C�e�B���O�L��
	//pDX->SetZBuffer(true);
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����

#ifdef BUILD_MODE
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	for (auto & mesh : m_aDebugMeshMap)
	{
		mesh.second.pMesh->DrawInstancing(mesh.second.mtx);
	}
	m_aDebugMeshMap.clear();
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
#endif // BUILD_MODE

}


#if BUILD_MODE

void CDrawSystemBase::ImGuiDebug()
{
	ImGui::Text("Renderer MAX : %d", m_aPolygonList.size());
	ImGui::Text(u8"Draw OK : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"Draw Skip : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text("Instancing Num : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"Renderer Sort", &m_bIsSortNecessary);

	//--- shadow �����_�[�^�[�Q�b�g�\��
	ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth()*0.3f, CScreen::GetHeight()*0.7f), ImGuiCond_::ImGuiCond_Once);
	if (ImGui::Begin("Shadow Depth", (bool*)m_bShadowView))
	{
		ImGui::Image(m_pDepthShadow->GetResource(), ImVec2(CScreen::GetWidth()*0.25f, CScreen::GetHeight()*0.25f));
		ImGui::End();
	}

	CShaderAssets::ImGuiDebug();

	//--- ���̃t���[���p������
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}

#endif // BUILD_MODE