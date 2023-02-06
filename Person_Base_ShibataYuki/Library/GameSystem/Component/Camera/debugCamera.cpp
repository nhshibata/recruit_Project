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

//==========================================================
// �R���X�g���N�^
//==========================================================
CDebugCamera::CDebugCamera()
	:m_eMode(ECameraMode::CAM_MODE_NONE), m_oldMousePos({ LONG(0),LONG(0) }), m_bMouse(true)
{
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CDebugCamera::CDebugCamera(std::shared_ptr<CGameObject> owner)
	: CCamera(owner), m_eMode(ECameraMode::CAM_MODE_NONE), m_oldMousePos({LONG(0),LONG(0)}), m_bMouse(true)
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CDebugCamera::~CDebugCamera()
{

}

//==========================================================
// �������Ăяo��
//==========================================================
void CDebugCamera::Awake()
{
	CCamera::Awake();

	// ���C����ׂ������ɐݒ�
	//GetOwner()->GetTagPtr()->CreateTag("DebugCamera");
	//GetOwner()->SetTag("DebugCamera");
	//SetMain(BaseToDerived<CCamera>());

#if BUILD_MODE
	ResumeCamera(true);
#endif // BUILD_MODE
}

//==========================================================
// ������
//==========================================================
void CDebugCamera::Init()
{
	CCamera::Init();
}

//==========================================================
// �X�V
//==========================================================
void CDebugCamera::Update()
{
	//--- �}�E�X�i�J�[�\��)�ɂ��X�V
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
		if (GetAsyncKeyState('A'))
		{
			pos -= Transform()->right() * MOVE_SPEED;
		}
		if (GetAsyncKeyState('D'))
		{
			pos += Transform()->right() * MOVE_SPEED;
		}
		if (GetAsyncKeyState('W'))
		{
			pos += Transform()->forward() * MOVE_SPEED;
		}
		if (GetAsyncKeyState('S'))
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

	if (!m_bMouse)
		m_eMode = ECameraMode::CAM_MODE_NONE;

	// �}�E�X�J�[�\���̎擾
	POINT pos;
	GetCursorPos(&pos);
	CameraMouseMove(pos.x, pos.y);
}

//==========================================================
// �ŏI�X�V
//==========================================================
void CDebugCamera::LateUpdate()
{
	CCamera::Update();
}

//==========================================================
// ��ב���
//==========================================================
void CDebugCamera::CameraMouseMove(int x, int y)
{
	//--- �J�����𓮂�����Ԃ��m�F
	if (m_eMode == ECameraMode::CAM_MODE_NONE)
	{
		// �}�E�X�̍��W�X�V
		m_oldMousePos.x = x;
		m_oldMousePos.y = y;
		return;
	}

	// �}�E�X�̈ړ���
	float mouseMoveX = (float)x - (float)m_oldMousePos.x;
	float mouseMoveY = (float)y - (float)m_oldMousePos.y;

	//--- �}�E�X�̍��W�X�V
	m_oldMousePos.x = x;
	m_oldMousePos.y = y;

	Vector3 pos = CCamera::GetPos();
	XMVECTOR vPos = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
	XMVECTOR vLook = XMVectorSet(m_vTarget.x, m_vTarget.y, m_vTarget.z, 0.0f);
	XMVECTOR vFront = XMVectorSubtract(vLook, vPos);
	XMVECTOR vUp = XMVector3Normalize(XMLoadFloat3(&m_vUp));
	XMVECTOR side;
	float focus = 0.0f;

	XMStoreFloat(&focus, XMVector3Length(vFront));
	//--- ���K��
	vFront = XMVector3Normalize(vFront);
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
		XMVECTOR rotAxis = XMVector3Normalize(XMVector3TransformCoord(side, rotY));
		
		XMMATRIX rotSide = XMMatrixRotationAxis(rotAxis, XMConvertToRadians(angleY));
		DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(vFront, focus);
		vRelative = XMVector3TransformCoord(vRelative, rotY * rotSide);

		// �����_�����_�Ƃ��ĉ�]
		/*rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotSide);
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(rotPos, vLook));*/
		
		// ���W�X�V
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorSubtract(vLook, vRelative));

		// �A�b�v�x�N�g�����v�Z
		vUp = DirectX::XMVector3Cross(
			DirectX::XMVector3Normalize(vRelative), rotAxis);
		DirectX::XMStoreFloat3(&m_vUp, DirectX::XMVector3Normalize(vUp));
		break;
	}
	case ECameraMode::CAM_MODE_TRACK:
	{
		float farZ = GetFarZ();

		// ���A�A����B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
		// ��L�������ƂɊ���o�������i�̈ړ��ʂƁA�t�H�[�J�X�ʒu / ���i �̔䗦����A�J�����̈ړ��ʂ����߂�
		float farScreenHeight = tanf(GetFOV() * 0.5f) * farZ;
		float farMoveX = -farScreenHeight * mouseMoveX / (CScreen::GetWidth() * 0.5f) * GetAspect();
		float farMoveY =  farScreenHeight * mouseMoveY / (CScreen::GetHeight() * 0.5f);
		
		// �p���s������ƂɃJ�������ړ�
		float rate = focus / farZ;
		DirectX::XMVECTOR vMove = XMVectorZero();
		vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(side, farMoveX * rate));
		vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(vUp, farMoveY * rate));

		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(vPos, vMove));
		DirectX::XMStoreFloat3(&m_vTarget, DirectX::XMVectorAdd(vLook, vMove));
	break;
	}
	case ECameraMode::CAM_MODE_DOLLY:
	{
		float farZ = GetFarZ();
		float rate = focus / farZ;
		XMVECTOR vMove = DirectX::XMVectorScale(vFront, farZ * 0.01f * rate * (mouseMoveY + mouseMoveX));
		XMStoreFloat3(&pos, DirectX::XMVectorAdd(vPos, vMove));
	break;
	}
	default:
	break;
	}

	Transform()->SetPos(pos);

	// �}�E�X�̍��W�X�V
	m_oldMousePos.x = x;
	m_oldMousePos.y = y;
}

#if BUILD_MODE

//==========================================================
// Debug�p
// Ҳݶ�ׂ̐ؑ�
//==========================================================
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

//==========================================================
// Debug�p reset
//==========================================================
void CDebugCamera::Reset()
{
	m_oldMousePos.x = LONG(0);
	m_oldMousePos.y = LONG(0);
	CCamera::Init();

}

#endif // BUILD_MODE