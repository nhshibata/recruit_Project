//=========================================================
//作成:2022/04/21
// プレイヤークラス
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <ImGui/imgui.h>

using namespace MySpace::Game;
using namespace MySpace::MyMath;

//--- 定数定義
namespace{
	const Vector3 LIGHT0_DIRECTION = Vector3(0.0f, -1.0f, 1.0f);
	const Vector4 LIGHT0_DIFFUSE = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	const Vector4 LIGHT0_AMBIENT = Vector4(0.4f, 0.4f, 0.4f, 1.0f);
	const Vector4 LIGHT0_SPECULAR = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
}

//--- ﾒﾝﾊﾞ関数
CDirectionalLight::CDirectionalLight(std::shared_ptr<CGameObject> owner)
	:CLight(owner)
{
}
CDirectionalLight::~CDirectionalLight()
{

}

void CDirectionalLight::Init()
{
	Set(this);
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	SetEnable();
}
XMFLOAT3& CDirectionalLight::GetDir()
{
	if (IsEnable()) 
		return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

#ifdef BUILD_MODE

void CDirectionalLight::ImGuiDebug()
{

}
#endif // BUILD_MODE