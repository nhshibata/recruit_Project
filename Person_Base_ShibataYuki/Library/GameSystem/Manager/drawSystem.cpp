//==========================================================
// [drawSystem.cpp]
//----------------------------------------------------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/09/11 �����䓖���蔻��𒲐�
// �X�V:2022/11/09 ���O�ύX(DrawManager) -> (drawSystem)
// �X�V:2022/12/15 �C���X�^���V���O�`��Ή�
// �X�V:2023/01/12 �C���X�^���V���O�`��̎����ɔ����o�O�C��
//----------------------------------------------------------
// �`��Ǘ�
//==========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>

#include <GraphicsSystem/Render/billboard.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <GraphicsSystem/Manager/effectManager.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>

#include <ImGui/imgui.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CDrawSystem::CDrawSystem()
	:m_bIsSortNecessary(true), m_bFrustum(true)
{
	m_aPolygonList.clear();
	m_aInstancingModelMap.clear();
	m_aInstancingMeshMap.clear();
	
#if BUILD_MODE
	m_nSkipCnt = m_nDrawCnt = m_nInstancingCnt = 0;
#endif // BUILD_MODE

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDrawSystem::~CDrawSystem()
{
}

//==========================================================
// �o�^
//==========================================================
int CDrawSystem::PolygonRegist(std::weak_ptr<CPolygonRenderer> render)
{
	//--- �A�b�v�L���X�g
	int ret = CMapSystemBase::RegistToSystem(render.lock());
	m_aPolygonList.push_back(render);
	return ret;
}

//==========================================================
// �o�^
//==========================================================
std::weak_ptr<CRenderer> CDrawSystem::ExecutSystem(int idx)
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
	CMapSystemBase::ExecutSystem(idx);
	return release;
}

//==========================================================
// �X�V
//==========================================================
void CDrawSystem::Update()
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


	//--- �ʏ�`��
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
		if (auto mesh = render.lock()->BaseToDerived<CMeshRenderer>().get(); mesh && m_bFrustum)
		{	
			float fRadius = mesh->GetBSRadius();
			auto mW = mesh->Transform()->GetWorldMatrix();
			fRadius = (mW._11 > fRadius) ? mW._11 : fRadius;
			fRadius = (mW._22 > fRadius) ? mW._22 : fRadius;
			fRadius = (mW._33 > fRadius) ? mW._33 : fRadius;
			
			//--- ��ׂɉf�邩����
			//if (CCamera::GetMain()->CollisionViewFrustum(&mesh->GetCenter(0), mesh->GetBSRadius()) == CCamera::EFrustumResult::OUTSIDE)
			if (CCamera::GetMain()->CollisionViewFrustum(&Vector3(mW._41, mW._42 ,mW._43), fRadius) == CCamera::EFrustumResult::OUTSIDE)
			{
#ifdef BUILD_MODE
				++m_nSkipCnt;
#endif // _DEBUG
				continue;
			}

			//--- �����܂ł�����`��
			render.lock()->Draw();
		}
	}

	//--- �C���X�^���V���O�����̂ŕ���
	InstancingDraw();

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
void CDrawSystem::Sort()
{
	//--- ����m�F(�����ŏ���erace�����̂͂�������)
	/*for (auto it = m_aPolygonList.begin(); it != m_aPolygonList.end();)
	{
		if (!(*it).lock())
		{
			it = m_aPolygonList.erase(it);
			continue;
		}
		++it;
	}*/

	if (m_aPolygonList.size() <= 1)
		return;
	
	//--- �����_���Ő���(����)
	std::sort(m_aPolygonList.begin(), m_aPolygonList.end(), [](auto const& s1, auto const& s2)->bool
	{
		return s1.lock()->GetZ() < s2.lock()->GetZ();
	});
	
}

//==========================================================
// �C���X�^���V���O�`��
//==========================================================
void CDrawSystem::InstancingDraw()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();

	pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	CLight* pLight = CLight::GetMain();
	pLight->SetDisable();			// ���C�e�B���O����
	//pDX->SetZBuffer(true);		// Z��������
	pDX->SetZWrite(true);			// Z��������

	//--- �o�^���ꂽ���f�����ʂɕ`��
	const int MAX_INSTANCING = 100;
	//--- �s�����`��
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(intancingModel.first);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly);
		pLight->SetEnable();

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	pLight->SetEnable();			// ���C�e�B���O�L��


	pDX->SetZWrite(false);			// Z��������

	//--- �����������`��
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
	
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(intancingModel.first);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eOpacityOnly);
		intancingModel.second.clear();	// �g�p�I��
	}
	
	pDX->SetZWrite(true);			// Z��������
	pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
	// �N���A
	m_aInstancingModelMap.clear();

	//--- ���b�V���C���X�^���V���O�`��
	for (auto & mesh : m_aInstancingMeshMap)
	{
		if (mesh.second.size() == 0)// �ꉞ�m�F
			continue;

		// �r���{�[�h���m�F
		CBillboard* bill = dynamic_cast<CBillboard*>(mesh.second[0]);
		if (bill)
		{
			auto image = pAssets->GetImageManager()->GetResource(mesh.first);
			auto tex = image ? image->GetSRV() : NULL;
			CMesh::DrawInstancing(mesh.second, tex, &bill->GetTextureMatrix());
		}
		else
		{
			CMesh::DrawInstancing(mesh.second);
		}
#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}
	// �N���A
	m_aInstancingMeshMap.clear();

	//--- �ݒ��߂�
	pLight->SetEnable();		// ���C�e�B���O�L��
	//pDX->SetZBuffer(true);
	//CDXDevice::Get()->SetBlendState(static_cast<int>(EBlendState::BS_NONE));		// ���u�����f�B���O����
}

#ifdef BUILD_MODE

void CDrawSystem::ImGuiDebug()
{
	ImGui::Text(u8"�`�惊�X�g�� : %d", m_aPolygonList.size());
	ImGui::Text(u8"�`��OK�� : %d", m_nDrawCnt);
	ImGui::SameLine();
	ImGui::Text(u8"�`��X�L�b�v�� : %d", m_nSkipCnt);
	ImGui::SameLine();
	ImGui::Checkbox("Culling ON/OFF", (bool*)&m_bFrustum);
	ImGui::Text(u8"�C���X�^���V���O�� : %d", m_nInstancingCnt);
	ImGui::Checkbox(u8"�`��\�[�gON", &m_bIsSortNecessary);

	//ImGui::Text("Resource/Model:%d", CModelManager::Get()->GetNameList().size());
	//ImGui::Text("Resource/Image:%d", CImageResourceManager::Get()->GetNameList().size());
	//ImGui::Text("Resource/Effekseer:%d", CEffekseer::Get()->GetNameList().size());

	//--- ���̃t���[���p������
	m_nDrawCnt = 0;
	m_nSkipCnt = 0;
	m_nInstancingCnt = 0;
}

#endif



#if POST_TEST

//==========================================================
// �R���X�g���N�^
//==========================================================
CDrawSystemMap::CDrawSystemMap()
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDrawSystemMap::~CDrawSystemMap()
{

}

//==========================================================
// �X�V
//==========================================================
void CDrawSystemMap::Update()
{
	// TODO:��ŕς���B���ۂ̓o�^���ꂽLayer���łȂ���Ζ��ʂ�����
	auto layer = static_cast<int>(CLayer::E_Layer::FOG);

	//--- ���C���[���ɕ`��
	for (int cnt = 0; cnt < layer; ++cnt)
	{
		if (!m_aLayerMap.count(cnt))
			continue;
		
		//--- �O����
		// �`���ύX����ʂ��|������
		// ���쐬:CPostProcess�N���X�����C���[�ɑ��݂��邩�m�F
		// ���݂��Ă���Ό��ʂ��|����?

		// �`�揈��
		m_aLayerMap[cnt].Update();
	}
}

#endif // POST_TEST
