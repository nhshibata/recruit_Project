//==========================================================
// [drawSystem.cpp]
//----------------------------------------------------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/09/11 �����䓖���蔻��𒲐�
// �X�V:2022/11/09 ���O�ύX(DrawManager) -> (drawSystem)
// �X�V:2022/12/15 �C���X�^���V���O�`��Ή�
// �X�V:2023/01/12 �C���X�^���V���O�`��̎����ɔ����o�O�C��
// �X�V:2023/02/08 �V�F�[�_�[����
//----------------------------------------------------------
// �`��Ǘ�
//==========================================================

//--- �C���N���[�h��
#include <algorithm>
#include <Application/Application.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Camera/stackCamera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Shader/shaderAssets.h>

#include <GraphicsSystem/Render/polygon.h>
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
CDrawSystem::CDrawSystem()
{
	

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDrawSystem::~CDrawSystem()
{
	
}

//==========================================================
// �X�V
//==========================================================
void CDrawSystem::Update()
{
	CDrawSystemBase::Update();
}

//==========================================================
// 3D�C���X�^���V���O�`��
//==========================================================
void CDrawSystem::GBufferDraw()
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	LPCSTR vsName[] = {
		"VS_Assimp",
		"VS_Mesh",
	};

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
		pSM->CallBackFuncAndBind(std::string(), vsName[0]);
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
		pSM->CallBackFuncAndBind(std::string(), vsName[0]);
		model->DrawInstancing(pDX->GetDeviceContext(), intancingModel.second, EByOpacity::eTransparentOnly, false);
	}

	pDX->SetZWrite(true);	// Z��������
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
		pSM->CallBackFuncAndBind(std::string(), vsName[1]);

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

}

//=========================================================
// Update����Ăяo�����֐��̃I�[�o�[���C�h
//=========================================================
void CDrawSystem::Draw3D()
{
#if BUILD_MODE
	// ��ׂ�������
	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
	{
		auto all = CCamera::GetAllCamera();
		for (auto & cam : all)
		{
			pCamera = cam->BaseToDerived<CStackCamera>();
			if (pCamera)
				break;
		}
	}
	if (!pCamera)
		return;
#else

	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
		return;

#endif // BUILD_MODE

	// ��ׂ����ɕ`��
	auto aCamera = pCamera->GetStackCamera();
	int idx = 0;
	std::vector<ID3D11ShaderResourceView*> aTex;

	do
	{
		//--- GBuffer�`��
		// �����_�[�^�[�Q�b�g�̐ݒ�
		pCamera->GetGBuffer()->SetUpMultiRenderTarget();

		// 3D�`��
		GBufferDraw();

		// �e��ø����̐ݒ�
		// MEMO:�����ŌĂяo���Ă����̂�ø����ݒ肳��Ȃ�
		pCamera->GetGBuffer()->SetUpTextures();

		//--- �|�X�g�v���Z�X
		auto aVolume = m_VolumeMgr.GetVolume(pCamera->GetMask());
		for (auto & vol : aVolume)
		{
			vol->GetEffect()->DrawSprite(pCamera->GetGBuffer());
			aTex.push_back(vol->GetEffect()->GetResource());
		}

		// �擪���珇�ɶ�ׂ�ݒ�
		if (idx < aCamera.size())
		{
			pCamera = aCamera[idx].lock();
			idx++;
		}
		else
			pCamera.reset();

	} while (pCamera);

	//--- ø�������
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	
	//pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());
	pDX->SwitchRender(pDX->GetRenderTargetView(), nullptr);
	
	if (aTex.size() == 0)
		return;

	//--- �`��ݒ�
	pDX->SetZBuffer(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(CScreen::GetSize());
	CPolygon::SetPos(XMFLOAT2(0,0));
	CPolygon::SetFrameSize(XMFLOAT2(1,1));
	CPolygon::SetUV(XMFLOAT2(0,0));
	CPolygon::SetAngle(0);
	
	CPolygon::SetTexture(aTex[0]);

	pDX->SetBlendState(EBlendState::BS_ADDITIVE);
	for (int cnt = 1; cnt < aTex.size(); ++cnt)
	{
		CPolygon::SetTexture(aTex[cnt]);
		CPolygon::Draw(pDX->GetDeviceContext());
	}

	CPolygon::SetSize(XMFLOAT2(1,1));
	pDX->SetBlendState(EBlendState::BS_NONE);
	pDX->SetZBuffer(true);

}


#ifdef BUILD_MODE

//void CDrawSystem::ImGuiDebug()
//{
//	
//}

void CDrawSystem::SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh)
{
	if (!m_aDebugMeshMap.count(name))
		m_aDebugMeshMap[name].pMesh = mesh;

	m_aDebugMeshMap[name].mtx.push_back(mtx);
}

#endif

