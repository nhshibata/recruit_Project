//=========================================================
// [gameCamera.h]
// �쐬:2022/07/19
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/GameObject/gameObject.h>
#include <Application/screen.h>
#include <CoreSystem/Input/Keyboard.h>
//#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;
using namespace MySpace::System;
//using namespace MySpace::SceneManager;

CDebugCamera::CDebugCamera()
{
}
CDebugCamera::CDebugCamera(std::shared_ptr<CGameObject> owner)
	:CCamera(owner), m_eMode(ECameraMode::CAM_MODE_NONE), m_oldMousePos({0}), m_bMouse(true)
{
}
CDebugCamera::~CDebugCamera()
{

}
void CDebugCamera::Awake()
{
	CCamera::Awake();

	// ���C����ׂ������ɐݒ�
	//GetOwner()->GetTagPtr()->CreateTag("DebugCamera");
	//GetOwner()->SetTag("DebugCamera");
	//SetMain(BaseToDerived<CCamera>());
	ResumeCamera(true);
}
void CDebugCamera::Init()
{
	CCamera::Init();
}
void CDebugCamera::Update()
{
	// �}�E�X�i�J�[�\��)�ɂ��X�V
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		m_eMode = ECameraMode::CAM_MODE_ORBIT;
	}
	else if (GetAsyncKeyState(VK_MBUTTON))
	{
		m_eMode = ECameraMode::CAM_MODE_TRACK;
	}
	else if (GetAsyncKeyState(VK_RBUTTON))
	{
		m_eMode = ECameraMode::CAM_MODE_DOLLY;

		auto pos = Transform()->GetPos();
		if (GetAsyncKeyState(0x8000 & 'A'))
		{
			pos -= Transform()->right() * MOVE_SPEED;
		}
		if (GetAsyncKeyState(0x8000 & 'D'))
		{
			pos += Transform()->right() * MOVE_SPEED;
		}
		if (GetAsyncKeyState(0x8000 & 'W'))
		{
			pos += Transform()->forward() * MOVE_SPEED;
		}
		if (GetAsyncKeyState(0x8000 & 'S'))
		{
			pos -= Transform()->forward() * MOVE_SPEED;
		}
		Transform()->SetPos(pos);
	}
	else
		m_eMode = ECameraMode::CAM_MODE_NONE;

	// �}�E�X����ؑ�
	if (Input::Keyboad::IsTrigger(VK_F1))
		m_bMouse ^= true;

	//if (CInput::GetKeyRelease(VK_C))

	if (!m_bMouse)
		m_eMode = ECameraMode::CAM_MODE_NONE;

	// �}�E�X�J�[�\���̎擾
	POINT pos;
	GetCursorPos(&pos);
	CameraMouseMove(pos.x, pos.y);
}
void CDebugCamera::LateUpdate()
{
	CCamera::Update();
}
void CDebugCamera::CameraMouseMove(int x, int y)
{
	// �J�����𓮂�����Ԃ��m�F
	if (m_eMode == ECameraMode::CAM_MODE_NONE)
	{
		// �}�E�X�̍��W�X�V
		m_oldMousePos.x = x;
		m_oldMousePos.y = y;
		return;
	}
	// �}�E�X�̈ړ���
	float mouseMoveX = x - (float)m_oldMousePos.x;
	float mouseMoveY = y - (float)m_oldMousePos.y;
#if 1
	Vector3 pos = CCamera::GetPos();
	XMVECTOR vPos = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
	XMVECTOR vLook = XMVectorSet(m_vTarget.x, m_vTarget.y, m_vTarget.z, 0.0f);
	XMVECTOR vFront = XMVectorSubtract(vLook, vPos);
	XMVECTOR vUp = XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f);
	XMVECTOR side;
	float focus = 0.0f;

	XMStoreFloat(&focus, XMVector3Length(vFront));
	// ���K��
	vFront = XMVector3Normalize(vFront);
	vUp = XMVector3Normalize(vUp);
	side = XMVector3Normalize(XMVector3Cross(vUp, vFront));
	vUp = XMVector3Normalize(XMVector3Cross(vFront, side));

	switch (m_eMode)
	{
	case ECameraMode::CAM_MODE_NONE:
		break;
	case ECameraMode::CAM_MODE_ORBIT:
	{
		//--- �I�[�r�b�g(��ʑ�(�����_)�𒆐S�ɉ�]���鑀��)
		// �}�E�X�̈ړ��� /��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��
		float angleX = 360.0f * mouseMoveX / CScreen::GetWidth();		// �l�̓X�N���[�����W�ɉ������p�x
		float angleY = 180.0f * mouseMoveY / CScreen::GetHeight();	// �l�̓X�N���[�����W�ɉ������p�x

		XMVECTOR rotPos = XMVectorSubtract(vPos, vLook);
		XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(angleX));
		XMVECTOR rotAxis = XMVector3TransformCoord(side, rotY);
		XMMATRIX rotX = XMMatrixRotationAxis(rotAxis, XMConvertToRadians(angleY));
		// �����_�����_�Ƃ��ĉ�]
		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(rotPos, vLook));

		// �A�b�v�x�N�g�����v�Z
		vUp = DirectX::XMVector3Cross(
			DirectX::XMVector3Normalize(DirectX::XMVectorScale(rotPos, -1.0f)), rotAxis);
		DirectX::XMStoreFloat3(&m_vUp, DirectX::XMVector3Normalize(vUp));
	}
	break;
	case ECameraMode::CAM_MODE_TRACK:
	{
		float farZ = GetFarZ() / 1000;
		float rate = focus / farZ;
		float aspect = GetAspect();
		// ���A�A����B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
		// ��L�������ƂɊ���o�������i�̈ړ��ʂƁA�t�H�[�J�X�ʒu / ���i �̔䗦����A�J�����̈ړ��ʂ����߂�
		float farMoveX = tanf(DirectX::XM_PI / 6.0f) * farZ * -mouseMoveX * aspect / (CScreen::GetWidth() * 0.5f);
		float farMoveY = tanf(DirectX::XM_PI / 6.0f) * farZ *  mouseMoveY / (CScreen::GetHeight() * 0.5f);
		// �p���s������ƂɃJ�������ړ�
		DirectX::XMVECTOR vMove = DirectX::XMVectorAdd(DirectX::XMVectorScale(side, farMoveX * rate), DirectX::XMVectorScale(vUp, farMoveY * rate));
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(vPos, vMove));
		DirectX::XMStoreFloat3(&m_vTarget, DirectX::XMVectorAdd(vLook, vMove));
	}
	break;
	case ECameraMode::CAM_MODE_DOLLY:
	{
		XMStoreFloat3(&pos, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * mouseMoveY * 0.01f)));
	}
	break;
	}

	//CCamera::SetPos(pos);
	Transform()->SetPos(pos);

#endif // 1

	// �}�E�X�̍��W�X�V
	m_oldMousePos.x = x;
	m_oldMousePos.y = y;
}

void CDebugCamera::ResumeCamera(bool bSwitch)
{
	static std::weak_ptr<CCamera> work;

	if (bSwitch)
	{
		// ���C����ׂɂȂ��ĂȂ���
		if (this != CCamera::GetMain())
		{
			work = CCamera::GetMain(0);
			SetMain(this->BaseToDerived<CCamera>());
		}
	}
	else
	{	// ���C���J������߂�
		if (work.lock())
			SetMain(work);
		else
		{
			if (auto obj = CGameObject::FindGameObjectWithTag(CDefaultTagChar::CAMERA); obj.lock())
			{
				CCamera::SetMain(obj.lock()->GetComponent<CCamera>());
			}
		}
	}
}