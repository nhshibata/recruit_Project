//==============================================
// [effect.cpp] 
//-----------------------
// 2021/12/13 �쐬 �ēc�E�M
// �Ƃ肠�����`��m�F
// �`��ꏊ�Ȃǂ̕ύX�\��
// 2022/01/25 �Đ��t���[���w�肵�����ꍇ�v�ύX
//==============================================

//--- �C���N���[�h��
#include <GraphicsSystem/Manager/effectManager.h>
#include <CoreSystem/Input/input.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <uchar.h>

// --- �萔��` ---
#define KARI_FILE	u"data/effect/Laser01.efkefc"

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

// --- �ÓI�����o�ϐ� ---
//EffekseerRendererDX11::RendererRef CEffekseer::m_renderer;
//Effekseer::ManagerRef CEffekseer::m_manager;
//Effekseer::Handle  CEffekseer::m_handle;

// �R���X�g���N�^
CEffekseer::CEffekseer()
{
}
// �f�X�g���N�^
CEffekseer::~CEffekseer()
{
}
// �V�[�����Ɉ�x�����Ăяo��
void CEffekseer::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	//hr = Load(device, context);
		// DirectX�ŉ�ʂɃI�u�W�F�N�g���\�������܂�
	// �@�I�u�W�F�N�g�̒��_���(�ʂ̍\�����)
	// �A���_���C�A�E�g(InputLayout)
	//   GPU�ɒ��_�\���������邽�߂̃f�[�^
	// �B���_�V�F�[�_(VertexShader)
	//   CPU����GPU�ɒ��_�f�[�^�𑗂�A3D��ԏ�̓_��
	//   2D��ʏ�̕\���ʒu�ɕϊ�����
	// �C�s�N�Z���V�F�[�_(PixelShader)
	//   ���_�V�F�[�_�ŕϊ������\���ʒu�ɁA
	//   �ǂ̂悤�ȐF���\������邩�w�肷��

	// �~���I�u�W�F�N�g�쐬
	struct CylinderVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
	const static int CylinderVtxCount = 16;
	CylinderVertex cylinderVtx[(CylinderVtxCount + 1) * 2];
	for (int i = 0; i <= CylinderVtxCount; ++i)
	{
		float rad = i * DirectX::XM_PI * 2.f / CylinderVtxCount;
		float u = static_cast<float>(i * 3.0f) / CylinderVtxCount;
		DirectX::XMFLOAT3 normal(sinf(rad), 0.0f, -cosf(rad));
		int idx = i * 2;
		// ��i
		cylinderVtx[idx + 0].normal = normal;
		cylinderVtx[idx + 0].pos = normal;
		cylinderVtx[idx + 0].pos.y = 1.0f;
		cylinderVtx[idx + 0].uv = DirectX::XMFLOAT2(u, 0.0f);
		// ���i
		cylinderVtx[idx + 1].normal = normal;
		cylinderVtx[idx + 1].pos = normal;
		cylinderVtx[idx + 1].pos.y = -1.0f;
		cylinderVtx[idx + 1].uv = DirectX::XMFLOAT2(u, 1.0f);
	}
	CylinderVertex polyVtx[4] = {
		{ DirectX::XMFLOAT3(-1, 1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(1, 1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(1, 0) },
		{ DirectX::XMFLOAT3(-1,-1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(0, 1) },
		{ DirectX::XMFLOAT3(1,-1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(1, 1) },
	};

	// �萔�o�b�t�@�쐬
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera) { hr = E_FAIL; return; }

	DirectX::XMStoreFloat4x4(&m_vsMatrix.world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.view, pCamera->GetLookAtMatrix());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.proj, pCamera->GetProjectionMatrix());

	//// �V�F�[�_�ǂݍ���
	//HRESULT hr = S_OK;
	//m_pSampleVS = new VertexShader();
	//hr = m_pSampleVS->Load("Assets/SampleVS.cso");
	//if (FAILED(hr)) { return hr; }
	//m_pSamplePS = new PixelShader();
	//hr = m_pSamplePS->Load("Assets/SamplePS.cso");
	//if (FAILED(hr)) { return hr; }

	// �e�N�X�`���̓ǂݍ���
	//m_pTexture = new TextureFromFile();
	//hr = m_pTexture->Load(
	//	"Assets/stoneRoad.jpg");
	//if (FAILED(hr)) { return hr; }
	//m_pDisolveTex = new TextureFromFile();
	//hr = m_pDisolveTex->Load(
	//	"Assets/stoneRoadN.png");
	//if (FAILED(hr)) { return hr; }

	// Effekseer������
	m_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 8000);
	m_manager = Effekseer::Manager::Create(8000);
	// �`�惂�W���[���̐ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	// �s��
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);
	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		90.0f / 180.0f * 3.14f, 16.0f / 9.0f, 1.0f, 500.0f));
	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(eye, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
	// 2�o�C�g�����̎擾
	//m_effect = Effekseer::Effect::Create(m_manager, u"data/Assets/wind.efkefc");
	//m_effect = Effekseer::Effect::Create(m_manager, u"data/Assets/ice.efk");

	Load(KARI_FILE);

	m_nTimer = 0;
}
void CEffekseer::Uninit()
{
	auto it = m_aResourceMap.begin();
	for (; it != m_aResourceMap.end(); ++it)
	{
		(*it).second.Reset();
	}
	m_aResourceMap.clear();
	
	// ShutDown the manager
	// �}�l�[�W���[�̔j��
	//if (m_manager.Get())
	//{
	//	m_manager.Get()->Release();
	//	//m_manager.Reset();
	//}

	// ShutDown the sound
	// �T�E���h�̔j��
	//m_sound.Reset();

	// ShutDown the renderer
	// �����_���[�̔j��
	/*if (m_renderer.Get())
	{
		m_renderer.Get()->Release();
	}*/
}

void CEffekseer::Update()
{
	// �G�t�F�N�g�̍Đ� 	
	//m_handle = m_manager->Play(m_effect[rand()%3][0], m_vPos.x, m_vPos.y, m_vPos.z);

	if (CInput::GetKeyTrigger(VK_Q))
	{
		Play(u"laser", DirectX::XMFLOAT3(0, 0, 0));
		// stop effects
		//m_manager->StopEffect(m_handle);
	}

	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;

	// �s��
	DirectX::XMFLOAT3 cameraPos = pCamera->GetPos();
	DirectX::XMFLOAT3 cameraLook = pCamera->GetTarget();
	DirectX::XMFLOAT3 cameraUp = pCamera->GetUpVector();
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(cameraPos.x, cameraPos.y, cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up = ::Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);
	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		pCamera->GetFOV(), pCamera->GetAspect(), pCamera->GetNearClip(), pCamera->GetFarZ()));
	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(eye, look, up));

	// ���C�g�̈ړ�
	//static float rad = 0.0f;

	//m_psLight = DirectX::XMFLOAT4(sinf(rad), 0, cosf(rad), 0);
	//rad += 0.01f;
	//m_pLightCB->Write(&m_psLight);

	//DirectX::XMFLOAT3 pos = pCamera->GetPos();
	//DirectX::XMFLOAT4 pos4(pos.x, pos.y, pos.z, 0.0f);
	//m_pCameraCB->Write(&pos4);

	EffectParam::iterator paramIt = m_effectParam.begin();
	while (paramIt != m_effectParam.end())
	{
		// �`�掞�Ԃ��I����Ă��邩�m�F
		if (!(*paramIt).IsPlay())	// �I����Ă���΃G�t�F�N�g�̒�~���߁A�p�����[�^���폜
		{
			m_manager->StopEffect((*paramIt).GetHandle());
			paramIt = m_effectParam.erase(paramIt);
			continue;
		}
		XMFLOAT3 pos = (*paramIt).GetPos();
		XMFLOAT4 rot = (*paramIt).GetRot();
		XMFLOAT3 size = (*paramIt).GetScale();
		Effekseer::Handle handle = (*paramIt).GetHandle();

		m_manager->SetLocation(handle, pos.x, pos.y, pos.z);
		m_manager->SetScale(handle, size.x, size.y, size.z);		// �T�C�Y�ύX

		m_manager->SetRotation(handle, Effekseer::Vector3D(rot.x, rot.y, rot.z), rot.w);

		// ����
		++paramIt;
	}

	// �X�V(���ׂẴG�t�F�N�g)
	// update the manager
	m_manager->Update();
}

void CEffekseer::Draw()
{
	// �G�t�F�N�g�̕`��
	m_renderer->SetTime(m_nTimer++ / 60.0f);	// �����̎��Ԃ�i�߂�

	// �`��
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState((int)EBlendState::BS_ALPHABLEND);
	m_renderer->BeginRendering();			// �`��J�n

	m_manager->Draw();

	m_renderer->EndRendering();				// �`��I��
	CDXDevice::Get().SetBlendState((int)EBlendState::BS_NONE);
	CDXDevice::Get().SetZBuffer(true);
}

void CEffekseer::SetMatrix(Effekseer::Matrix43* mtx, XMFLOAT4X4 XMmtx)
{
	mtx->Value[0][0] = XMmtx._11;
	mtx->Value[0][1] = XMmtx._12;
	mtx->Value[0][2] = XMmtx._13;

	mtx->Value[1][0] = XMmtx._21;
	mtx->Value[1][1] = XMmtx._22;
	mtx->Value[1][2] = XMmtx._23;

	mtx->Value[2][0] = XMmtx._31;
	mtx->Value[2][1] = XMmtx._32;
	mtx->Value[2][2] = XMmtx._33;

	mtx->Value[3][0] = XMmtx._41;
	mtx->Value[3][1] = XMmtx._42;
	mtx->Value[3][2] = XMmtx._43;
}
// kari
bool CEffekseer::Load(std::u16string fileName)
{
	if (m_aResourceMap.find(fileName.c_str()) != m_aResourceMap.end())
	{
		return false;
	}
	
	m_aResourceMap.insert(EffectMapPair(fileName.c_str(), Effekseer::Effect::Create(m_manager, fileName.c_str())
	));
	return true;
}
// ̧�ٓǂݍ��ݗp
bool CEffekseer::Load(std::vector<std::string> EffectName, std::vector<std::u16string> fileName)
{
	//std::vector<std::string>::iterator nameIt = EffectName.begin();
	//std::vector<std::u16string>::iterator fileIt = fileName.begin();
	//// �i�[����Ă���t�@�C�������󂯎��A�G�t�F�N�g���쐬
	//while (fileIt != fileName.end())
	//{
	//	// �ǂݍ���łȂ������ׂ�
	//	if (m_Effect.find((*nameIt).c_str()) != m_Effect.end())
	//	{
	//		// �L�[���������� = ���ɓǂݍ��܂ꂽ�t�@�C��
	//		++nameIt;
	//		++fileIt;
	//		continue;
	//	}
	//	// �t�@�C�����ƃG�t�F�N�g��ǉ�
	//	m_Effect.insert(EffectMapPair((*nameIt).c_str(), Effekseer::Effect::Create(m_manager, (*fileIt).c_str())
	//	));

	//	++nameIt;
	//	++fileIt;
	//}
	return true;
}
// �ݒ肵�����O�̃G�t�F�N�g���Đ�
int CEffekseer::Play(std::u16string effectName, XMFLOAT3 pos)
{
	// �ݒ肳�ꂽ�G�t�F�N�g����Ȃ���Ώ�����Ԃ�
	if (m_aResourceMap.find(effectName) == m_aResourceMap.end())	// find�̕Ԓl���I�[�Ȃ珈����Ԃ�
	{
		return -1;
	}
	// �Đ�
	Effekseer::Handle handle = m_manager->Play(m_aResourceMap[effectName], pos.x, pos.y, pos.z);
	m_effectParam.push_back(CEffekseerParam(&m_aResourceMap[effectName], handle,
		m_aResourceMap[effectName]->CalculateTerm().TermMax));

	return handle;
}
// �ݒ肵�����O�̃G�t�F�N�g���Đ�
int CEffekseer::Play(std::u16string effectName, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4 rot)
{
	// �ݒ肳�ꂽ�G�t�F�N�g����Ȃ���Ώ�����Ԃ�
	if (m_aResourceMap.find(effectName) == m_aResourceMap.end())	// find�̕Ԓl���I�[�Ȃ珈����Ԃ�
	{
		return -1;
	}
	// �Đ�
	Effekseer::Handle handle = m_manager->Play(m_aResourceMap[effectName], pos.x, pos.y, pos.z);
	// �p�����[�^�����B�G�t�F�N�g�̃|�C���^�ƍĐ������G�t�F�N�g�̃n���h����n��
	m_effectParam.push_back(CEffekseerParam(&m_aResourceMap[effectName], handle, m_aResourceMap[effectName]->CalculateTerm().TermMax));
	m_effectParam.back().SetPos(pos);
	m_effectParam.back().SetScale(size);
	m_effectParam.back().SetRot(rot);

	m_manager->SetScale(handle, size.x, size.y, size.z);		// �T�C�Y�ύX
	m_manager->SetRotation(handle, Effekseer::Vector3D(rot.x, rot.y, rot.z), rot.w);
	return handle;
}
// ��~
int CEffekseer::Stop(int handle)
{
	m_manager->StopEffect(handle);

	return -1;
}
int CEffekseer::Move(int handle, XMFLOAT3 pos)
{
	if (!m_manager->Exists(handle))
	{
		return -1;
	}

	// �ړ�
	Effekseer::Matrix43 mtx;
	m_vsMatrix.view._41 = pos.x;
	m_vsMatrix.view._42 = pos.y;
	m_vsMatrix.view._43 = pos.z;
	// Effekseer�̍\���̂ɕϊ�
	SetMatrix(&mtx, m_vsMatrix.view);
	m_manager->SetMatrix(handle, mtx);		// �s��ݒ�
	
	return handle;
}