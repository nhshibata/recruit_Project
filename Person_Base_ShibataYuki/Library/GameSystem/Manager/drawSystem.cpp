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
// �`��Ώۂ̊m�F
// Base�Ƃقړ��������ALayer���ǉ�����Avolume�֊i�[���Ă���
//==========================================================
void CDrawSystem::CheckRenderedObjectsIn3D()
{
	// �L���b�V���̃N���A
	m_VolumeMgr.ResetRendererCash();

	//--- 3D�`��(Mesh,Model)
	for (auto & it : m_aIntMap)
	{
		auto render = it.second.lock();

		// �|�C���^�m�F
		if (!render)
			continue;

		// �`��\�ȏ�Ԃ��m�F
		if (!render->IsActive() || !render->IsVisible())
			continue;

		// layer�m�F
		if (!CCamera::GetMain()->IsMask(render->GetLayer()))
			continue;

#ifdef BUILD_MODE
		++m_nDrawCnt;
		auto name = render->GetName();
#endif // _DEBUG

		//--- Mesh���߰���(����ьp��)���m�F
		// �J�����O�t���O���m�F
		if (auto mesh = render->BaseToDerived<CMeshRenderer>().get(); mesh)
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
				render->GetOwner()->CameraTest(false);
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
		render->Draw();
		render->GetOwner()->CameraTest(true);

		// ����Layer��ID���i�[
		m_VolumeMgr.AddRendererCash(CLayer::NumberToBit(render->GetLayer()), it.first);
	}
}

//==========================================================
// 3D�C���X�^���V���O�`��
//==========================================================
void CDrawSystem::GBufferDraw(const bool bGbuffer, std::function<bool(int)> func)
{
	auto pAssets = Application::Get()->GetSystem<CAssetsManager>();
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto pSM = Application::Get()->GetSystem<CAssetsManager>()->GetShaderManager();
	LPCSTR aVSName[] = {
		"VS_Assimp",
		"VS_Mesh",
	};
	CLight* pLight = CLight::GetMain();

	if (!bGbuffer)
	{
		//pLight->SetDisable();
		pDX->SetCullMode((int)ECullMode::CULLMODE_CCW);
	}

	//--- �o�^���ꂽ���f�����ʂɕ`��
	//--- �s�����`��
	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = TextSplitToNamePSVS(intancingModel.first);
		if (aName.IsError())
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(aName.strName);
		// ���f�����������Ă��Ȃ����ꉞ�m�F
		if (!model)
			continue;

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[0]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eOpacityOnly, false);

#if BUILD_MODE
		++m_nInstancingCnt;
#endif // _DEBUG
	}

	if (!bGbuffer)
	{
		// ���C�e�B���O�L��
		pLight->SetEnable();

		//--- �����������`��
		pDX->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));
		pDX->SetZWrite(false);// Z��������
	}

	for (auto & intancingModel : m_aInstancingModelMap)
	{
		auto aName = TextSplitToNamePSVS(intancingModel.first);
		if (aName.IsError())
			continue;

		//--- �`�悷�郂�f���̎擾
		auto model = pAssets->GetModelManager()->GetModel(aName.strName);

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[0]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : intancingModel.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}

			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		model->DrawInstancing(pDX->GetDeviceContext(), data, EByOpacity::eTransparentOnly, false);
	}

	if (!bGbuffer)
	{
		pDX->SetZWrite(true);	// Z��������
		pDX->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
	}

	//--- ���b�V���C���X�^���V���O�`��
	for (auto & meshObj : m_aInstancingMeshMap)
	{
		if (meshObj.second.aID.size() == 0)// �ꉞ�m�F
			continue;

		auto aName = TextSplitToNamePSVS(meshObj.first);
		if (aName.IsError())
			continue;

		// shaderBind
		if(bGbuffer)
			pSM->CallBackFuncAndBind(std::string(), aVSName[1]);
		else
			pSM->CallBackFuncAndBind(aName.strPixel, aName.strVertex);

		//--- �C���X�^���V���O�ɕK�v�ȃf�[�^�i�[
		std::vector<RENDER_DATA> data;
		for (auto & id : meshObj.second.aID)
		{
			if (func)
			{
				if (!func(id))
					continue;
			}
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			data.push_back(mesh->GetShaderData());
		}

		// �r���{�[�h���m�F
		if (CBillboard* bill = dynamic_cast<CBillboard*>(meshObj.second.pMesh); bill != nullptr)
		{
			// ø����ݒ�
			auto image = pAssets->GetImageManager()->GetResource(aName.strName);
			auto tex = image ? image->GetSRV() : NULL;
			meshObj.second.pMesh->DrawInstancing(data, false, tex, &bill->GetTextureMatrix());
		}
		else
		{
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

//=========================================================
// Update����Ăяo�����֐��̃I�[�o�[���C�h
//=========================================================
void CDrawSystem::Draw3D()
{

#if BUILD_MODE	// ImGui�\������DebugCamera��Main�Ȃ̂ŁAhierarchy��T��
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
	{
		CDrawSystemBase::Draw3D();
		return;
	}
#else

	auto pCamera = CCamera::GetMain()->BaseToDerived<CStackCamera>();
	if (!pCamera)
	{
		CDrawSystemBase::Draw3D();
		return;
	}

#endif // BUILD_MODE

	// ��ׂ����ɕ`��
	auto pDX = Application::Get()->GetSystem<CDXDevice>();
	auto aCamera = pCamera->GetStackCameras();
	int idxCnt = 0;
	std::vector<ID3D11ShaderResourceView*> aEffectTex;

	do
	{
		//--- GBuffer���K�v��Layer���擾
		const int layerBit = m_VolumeMgr.GetBit(pCamera->GetMask());
		// �����_��
		// �`������ݒ���`
		auto layerChaeck = [=](int id)->bool {
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			return (CLayer::NumberToBit(mesh->GetLayer()) & layerBit);
		};
		auto layerChaeckCamera = [=](int id)->bool {
			CMeshRenderer* mesh = dynamic_cast<CMeshRenderer*>(m_aIntMap[id].lock().get());
			return (CLayer::NumberToBit(mesh->GetLayer()) & pCamera->GetMask());
		};

		//--- GBuffer�`��
		// �����_�[�^�[�Q�b�g�̐ݒ�
		pCamera->GetGBuffer()->SetUpMultiRenderTarget();
		// 3D�`��
		GBufferDraw(true, layerChaeck);

		// Scene�ɕ`����ύX
		pDX->SwitchRender(pDX->GetRenderTargetView(), pDX->GetDepthStencilView());
		// 3D�`��
		pCamera->GetGBuffer()->SetSRV(CGBuffer::ETexture::DEPTH);
		GBufferDraw(false, layerChaeckCamera);

		// �e��ø����̐ݒ�
		// NOTE:�����ŌĂяo���Ă����̂�ø����ݒ肳��Ȃ�
		//pCamera->GetGBuffer()->SetUpTextures();
		// ø����̺�߰
		// NOTE:�X�N���[���p��Texture����肭�ݒ肳��Ă��Ȃ�?
		//pCamera->GetGBuffer()->CopyTexture();

		//--- �|�X�g�v���Z�X
		auto aVolume = m_VolumeMgr.GetVolume(pCamera->GetMask());
		for (auto & vol : aVolume)
		{
			if (1)
			{
				auto aID = vol->GetRenderCash();
				// �����_��
				auto idChaeck = [=](int id)->bool {
					for (auto & no : aID)
					{
						if (no == id)
							return true;
					}
					return false;
				};
				//--- GBuffer�`��
				// �����_�[�^�[�Q�b�g�̐ݒ�
				pCamera->GetGBuffer()->SetUpColorRenderTarget();
				//pCamera->GetGBuffer()->SetUpMultiRenderTarget();
				// 3D�`��
				GBufferDraw(true, idChaeck);
			}
			vol->GetEffect()->DrawSprite(pCamera->GetGBuffer());
			aEffectTex.push_back(vol->GetEffect()->GetResource());
		}

		// �擪���珇�ɶ�ׂ�ݒ�
		if (idxCnt < aCamera.size())
		{
			pCamera = aCamera[idxCnt].lock();
			idxCnt++;
		}
		else
		{
			pCamera.reset();
		}

	} while (pCamera);

	//--- �����_�[�^�[�Q�b�g���f�t�H���g�ɖ߂�
	pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());
	//pDX->SwitchRender(pDX->GetRenderTargetView(), nullptr);
	
	// volume����؂Ȃ��̂ŏ������Ȃ�
	if (aEffectTex.size() == 0)
		return;

	//--- ø�������
	//--- �`��ݒ�
	pDX->SetZBuffer(false);
	pDX->SetBlendState(EBlendState::BS_ALPHABLEND);
	CPolygon::SetSize(CScreen::GetSize());
	CPolygon::SetPos(XMFLOAT2(0,0));
	CPolygon::SetFrameSize(XMFLOAT2(1,1));
	CPolygon::SetUV(XMFLOAT2(0,0));
	CPolygon::SetAngle(0);
	
	CPolygon::SetTexture(aEffectTex[0]);
	//CPolygon::Draw(pDX->GetDeviceContext());

	// ���Z�����K�{
	pDX->SetBlendState(EBlendState::BS_ADDITIVE);

	// �|���S���`��
	for (int cnt = 0; cnt < aEffectTex.size(); ++cnt)
	{
		CPolygon::SetTexture(aEffectTex[cnt]);
		CPolygon::Draw(pDX->GetDeviceContext());
	}

	CPolygon::SetSize(XMFLOAT2(1,1));
	pDX->SetBlendState(EBlendState::BS_NONE);
	pDX->SetZBuffer(true);

	pDX->SwitchRender(pDX->GetRenderTargetView(),pDX->GetDepthStencilView());

}


#ifdef BUILD_MODE

//void CDrawSystem::ImGuiDebug()
//{
//	
//}

#endif

