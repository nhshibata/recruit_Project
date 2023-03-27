//=========================================================
// [drawSystem.cpp]
// �h���N���X
//---------------------------------------------------------
// 
//---------------------------------------------------------
// drawSystem�������Ȃ����̂ƁA
// ���V�X�e����傫���ς���K�v���Ȃ��Ǝv���A���N���X������
//=========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystemBase.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/stackCamera.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>

#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/shaderManager.h>

#include <DebugSystem/imGuiPackage.h>

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

}

//==========================================================
// ������
//==========================================================
void CDrawSystemBase::Init()
{
	m_pDepthShadow->InitShader();

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

	// �`��Ώۂ̊m�F
	CheckRenderedObjectsIn3D();

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

#ifdef BUILD_MODE
	//--- �����蔻�胁�b�V���`��
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	for (auto & name : m_aDebugMeshMap)
	{
		std::vector<XMFLOAT4X4> inMtx;
		for (auto & mesh : name.second)
		{
			inMtx.push_back(mesh->GetWorld());
		}
		name.second[0]->DrawInstancing(inMtx, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
#endif // BUILD_MODE

}

//==========================================================
// �`��Ώۂ̊m�F
//==========================================================
void CDrawSystemBase::CheckRenderedObjectsIn3D()
{
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

		// layer�m�F
		if (!CCamera::GetMain()->IsMask(render.lock()->GetLayer()))
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
void CDrawSystemBase::SetInstanchingModel(std::string name, std::string ps, std::string vs, const int id)
{
	CDrawSystemBase::STDrawInfo add = STDrawInfo(name, ps, vs);
	m_aInstancingModelMap[add].aID.push_back(id);
}

//==========================================================
// �o�^
//==========================================================
void CDrawSystemBase::SetInstanchingMesh(std::string name, std::string ps, std::string vs, const int id, CMesh* mesh)
{
	CDrawSystemBase::STDrawInfo add = STDrawInfo(name, ps, vs);
	if (!m_aInstancingMeshMap.count(add))
		m_aInstancingMeshMap[add].pMesh = mesh;

	m_aInstancingMeshMap[add].aID.push_back(id);
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
const CDrawSystemBase::STDrawInfo CDrawSystemBase::TextSplitToNamePSVS(const std::string name)
{
	CDrawSystemBase::STDrawInfo ret;
	// ps���W�擾
	auto startIndex = name.find("#");
	// ���O�i�[
	ret.strName = name.substr(0, startIndex);
	startIndex += 1;
	// vs���W�擾
	auto endIndex = name.find("#", startIndex);
	//--- �����؂�o���i�[
	ret.strPixel = name.substr(startIndex, endIndex - startIndex);
	endIndex += 1;
	ret.strVertex = name.substr(endIndex, name.size() - (startIndex + endIndex));
	return ret;
}

//==========================================================
// ���O
//==========================================================
bool CDrawSystemBase::ExecutSystem(const int idx)
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
	for (auto & modelObj : m_aInstancingModelMap)
	{
		// �ꎞ�ϐ�
		auto info = &modelObj.first;

		if (info->IsError())
			continue;

		//--- �`�悷�郂�f���̎擾
		auto pModel = pAssets->GetModelManager()->GetModel(info->strName);
		// ���f�����������Ă��Ȃ����ꉞ�m�F
		if (!pModel)
			continue;

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : modelObj.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		pModel->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);
	}

	//--- Mesh
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		if (meshObj.second.aID.size() == 0)// �ꉞ�m�F
			continue;

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : meshObj.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// �r���{�[�h���m�F
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			// �ꎞ�ϐ�
			auto info = &meshObj.first;
			if (info->IsError())
				continue;

			// ø����ݒ�
			auto image = pAssets->GetImageManager()->GetResource(info->strName);
			auto tex = image ? image->GetSRV() : NULL;

			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			meshObj.second.pMesh->DrawInstancing(data, false);
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
		// �ꎞ�ϐ�
		auto info = &intancingModel.first;

		if (info->IsError())
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(info->strName);
		// ���f�����������Ă��Ȃ����ꉞ�m�F
		if (!model)
			continue;

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// shaderBind
		pSM->CallBackFuncAndBind(info->strPixel, info->strVertex);
		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	pLight->SetEnable();			// ���C�e�B���O�L��


	//--- �����������`��
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	pDX->SetZWrite(false);			// Z��������

	for (auto & modelObj : m_aInstancingModelMap)
	{
		// �ꎞ�ϐ�
		auto info = &modelObj.first;
		if (info->IsError())
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(info->strName);

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : modelObj.second.aID)
		{
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// shaderBind
		pSM->CallBackFuncAndBind(info->strPixel, info->strVertex);
		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eTransparentOnly, false);
	}

	pDX->SetZWrite(true);			// Z��������
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));

	//--- ���b�V���C���X�^���V���O�`��
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		// �ꎞ�ϐ�
		auto info = &meshObj.first;
		if (info->IsError())
			continue;

		// shaderBind
		pSM->CallBackFuncAndBind(info->strPixel, info->strVertex);

		// �r���{�[�h���m�F
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			// ø����ݒ�
			auto image = pAssets->GetImageManager()->GetResource(info->strName);
			auto tex = image ? image->GetSRV() : NULL;
			
			//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
			//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
			std::vector<RENDER_DATA> data;
			for (auto & id : meshObj.second.aID)
			{
				CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
				data.push_back(mesh->GetShaderData());
			}

			meshObj.second.pMesh->DrawInstancing(data, false);
		}
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}

	//--- �ݒ��߂�
	pLight->SetEnable();		// ���C�e�B���O�L��
	//pDX->SetZBuffer(true);
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����

}


#if BUILD_MODE

void CDrawSystemBase::ImGuiDebug()
{
	Debug::SetTextAndAligned("Renderer MAX");
	ImGui::Text("%d", m_aPolygonList.size());

	Debug::SetTextAndAligned("Visible ON");
	ImGui::Text("%d", m_nDrawCnt);

	Debug::SetTextAndAligned("Draw Skip");
	ImGui::Text("%d", m_nSkipCnt);
	
	Debug::SetTextAndAligned("Instancing Num");
	ImGui::Text("%d", m_nInstancingCnt);

	Debug::SetTextAndAligned("Polygon Renderer Sort");
	ImGui::Checkbox("##Renderer Sort", &m_bIsSortNecessary);

	Debug::SetTextAndAligned("Culling ON/OFF");
	ImGui::Checkbox("##Culling ON/OFF", (bool*)&m_bFrustum);

	Debug::SetTextAndAligned("Shadow Window");
	ImGui::Checkbox("##Shadow Window", &m_bShadowView);

	//--- shadow �����_�[�^�[�Q�b�g�\��
	if (m_pDepthShadow != nullptr && m_bShadowView)
	{
		ImGui::SetNextWindowPos(ImVec2(CScreen::GetWidth()*0.3f, CScreen::GetHeight()*0.7f), ImGuiCond_::ImGuiCond_Once);
		if (ImGui::Begin("Shadow Depth", &m_bShadowView))
		{
			m_pDepthShadow->ImGuiDebug();
			ImGui::End();
		}
	}

	//--- ���̃t���[���p������
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}

void CDrawSystemBase::SetDebugMesh(std::string name, CMesh* mesh)
{
	m_aDebugMeshMap[name].push_back(mesh);
}

void CDrawSystemBase::ReleaseDebugMesh(const CMesh* pMesh)
{
	for (auto & name : m_aDebugMeshMap)
	{
		for (auto & mesh : name.second)
		{
			if (pMesh != mesh)
				continue;

			// ����̃��b�V���߲�����������̂ŏ��O
			auto it = std::find(name.second.begin(), name.second.end(), mesh);
			name.second.erase(it);

			// 0�ɂȂ��Ă�map�͎c��̂ŁA���O
			if (name.second.size() == 0)
			{
				m_aDebugMeshMap.erase(name.first);
			}
			return;
		}
	}
}

#endif // BUILD_MODE
