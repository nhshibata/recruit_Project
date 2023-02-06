//=========================================================
// [gameCamera.h]
// 作成:2022/07/19
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/GameObject/gameObject.h>
#include <Application/screen.h>
#include <CoreSystem/Input/Keyboard.h>
//#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;
using namespace MySpace::System;

//==========================================================
// コンストラクタ
//==========================================================
CDebugCamera::CDebugCamera()
	:m_eMode(ECameraMode::CAM_MODE_NONE), m_oldMousePos({ LONG(0),LONG(0) }), m_bMouse(true)
{
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CDebugCamera::CDebugCamera(std::shared_ptr<CGameObject> owner)
	: CCamera(owner), m_eMode(ECameraMode::CAM_MODE_NONE), m_oldMousePos({LONG(0),LONG(0)}), m_bMouse(true)
{
}

//==========================================================
// デストラクタ
//==========================================================
CDebugCamera::~CDebugCamera()
{

}

//==========================================================
// 生成時呼び出し
//==========================================================
void CDebugCamera::Awake()
{
	CCamera::Awake();

	// メインｶﾒﾗを自分に設定
	//GetOwner()->GetTagPtr()->CreateTag("DebugCamera");
	//GetOwner()->SetTag("DebugCamera");
	//SetMain(BaseToDerived<CCamera>());

#if BUILD_MODE
	ResumeCamera(true);
#endif // BUILD_MODE
}

//==========================================================
// 初期化
//==========================================================
void CDebugCamera::Init()
{
	CCamera::Init();
}

//==========================================================
// 更新
//==========================================================
void CDebugCamera::Update()
{
	//--- マウス（カーソル)による更新
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

	// マウス操作切替
	if (Input::Keyboad::IsTrigger(VK_F1))
		m_bMouse ^= true;

	if (!m_bMouse)
		m_eMode = ECameraMode::CAM_MODE_NONE;

	// マウスカーソルの取得
	POINT pos;
	GetCursorPos(&pos);
	CameraMouseMove(pos.x, pos.y);
}

//==========================================================
// 最終更新
//==========================================================
void CDebugCamera::LateUpdate()
{
	CCamera::Update();
}

//==========================================================
// ｶﾒﾗ操作
//==========================================================
void CDebugCamera::CameraMouseMove(int x, int y)
{
	//--- カメラを動かす状態か確認
	if (m_eMode == ECameraMode::CAM_MODE_NONE)
	{
		// マウスの座標更新
		m_oldMousePos.x = x;
		m_oldMousePos.y = y;
		return;
	}

	// マウスの移動量
	float mouseMoveX = (float)x - (float)m_oldMousePos.x;
	float mouseMoveY = (float)y - (float)m_oldMousePos.y;

	//--- マウスの座標更新
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
	//--- 正規化
	vFront = XMVector3Normalize(vFront);
	side = XMVector3Normalize(XMVector3Cross(vUp, vFront));
	vUp = XMVector3Normalize(XMVector3Cross(vFront, side));

	switch (m_eMode)
	{
	case ECameraMode::CAM_MODE_NONE:
		break;

	case ECameraMode::CAM_MODE_ORBIT:
	{
		//--- オービット(被写体(注視点)を中心に回転する操作)
		// マウスの移動量 /画面サイズ の比率から、画面全体でどれだけ回転するか指定する
		float angleX = 360.0f * mouseMoveX / CScreen::GetWidth();		// 値はスクリーン座標に応じた角度
		float angleY = 180.0f * mouseMoveY / CScreen::GetHeight();	// 値はスクリーン座標に応じた角度

		XMVECTOR rotPos = XMVectorSubtract(vPos, vLook);
		XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(angleX));
		XMVECTOR rotAxis = XMVector3Normalize(XMVector3TransformCoord(side, rotY));
		
		XMMATRIX rotSide = XMMatrixRotationAxis(rotAxis, XMConvertToRadians(angleY));
		DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(vFront, focus);
		vRelative = XMVector3TransformCoord(vRelative, rotY * rotSide);

		// 注視点を原点として回転
		/*rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
		rotPos = DirectX::XMVector3TransformCoord(rotPos, rotSide);
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(rotPos, vLook));*/
		
		// 座標更新
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorSubtract(vLook, vRelative));

		// アップベクトルを計算
		vUp = DirectX::XMVector3Cross(
			DirectX::XMVector3Normalize(vRelative), rotAxis);
		DirectX::XMStoreFloat3(&m_vUp, DirectX::XMVector3Normalize(vUp));
		break;
	}
	case ECameraMode::CAM_MODE_TRACK:
	{
		float farZ = GetFarZ();

		// 底辺A、高さBとする三角形について tanΘ = A / Bが成り立つ
		// 上記式をもとに割り出した遠景の移動量と、フォーカス位置 / 遠景 の比率から、カメラの移動量を求める
		float farScreenHeight = tanf(GetFOV() * 0.5f) * farZ;
		float farMoveX = -farScreenHeight * mouseMoveX / (CScreen::GetWidth() * 0.5f) * GetAspect();
		float farMoveY =  farScreenHeight * mouseMoveY / (CScreen::GetHeight() * 0.5f);
		
		// 姿勢行列をもとにカメラを移動
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

	// マウスの座標更新
	m_oldMousePos.x = x;
	m_oldMousePos.y = y;
}

#if BUILD_MODE

//==========================================================
// Debug用
// ﾒｲﾝｶﾒﾗの切替
//==========================================================
void CDebugCamera::ResumeCamera(bool bSwitch)
{
	static std::weak_ptr<CCamera> work;

	if (bSwitch)
	{
		// メインｶﾒﾗになってない時
		if (this != CCamera::GetMain())
		{
			work = CCamera::GetMain(0);
			SetMain(this->BaseToDerived<CCamera>());
		}
	}
	else
	{	// メインカメラを戻す
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
// Debug用 reset
//==========================================================
void CDebugCamera::Reset()
{
	m_oldMousePos.x = LONG(0);
	m_oldMousePos.y = LONG(0);
	CCamera::Init();

}

#endif // BUILD_MODE