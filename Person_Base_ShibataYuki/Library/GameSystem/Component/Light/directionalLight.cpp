//=========================================================
//作成:2022/04/21
// プレイヤークラス
// 
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::MyMath;

//--- 定数定義
namespace
{
	const Vector3 LIGHT0_DIRECTION = Vector3(0.0f, -1.0f, 1.0f);
	const Vector4 LIGHT0_DIFFUSE = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	const Vector4 LIGHT0_AMBIENT = Vector4(0.4f, 0.4f, 0.4f, 1.0f);
	const Vector4 LIGHT0_SPECULAR = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
}

//=========================================================
// コンストラクタ
//=========================================================
CDirectionalLight::CDirectionalLight()
{
	Set(this);
}

//=========================================================
// 引数付きコンストラクタ
//=========================================================
CDirectionalLight::CDirectionalLight(std::shared_ptr<CGameObject> owner)
	:CLight(owner)
{
	Set(this);
}

//=========================================================
// デストラクタ
//=========================================================
CDirectionalLight::~CDirectionalLight()
{

}

//=========================================================
// 生成時呼び出し
//=========================================================
void CDirectionalLight::Awake()
{
	CLight::Awake();
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMFLOAT3 vDirection = LIGHT0_DIRECTION;
	XMStoreFloat3(&vDirection, XMVector3Normalize(XMLoadFloat3(&vDir)));
	this->Transform()->SetRot(vDirection);

	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	SetEnable();
}

//=========================================================
// 方向
//=========================================================
XMFLOAT3 CDirectionalLight::GetDir()
{
	if (IsEnable())
	{
		XMFLOAT3 vDirection = this->Transform()->GetRot();
		return vDirection;
	}
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

#ifdef BUILD_MODE

void CDirectionalLight::ImGuiDebug()
{
	CLight::ImGuiDebug();

	ImGui::Separator();

	Debug::SetTextAndAligned("light Ambient");
	ImGui::DragFloat4("##light Ambient", (float*)&m_ambient, 0.01f);
	
	Debug::SetTextAndAligned("light Diffuse");
	ImGui::DragFloat3("##light Diffuse", (float*)&m_diffuse, 0.01f);
	
	Debug::SetTextAndAligned("light Specular");
	ImGui::DragFloat3("##light Specular", (float*)&m_specular, 0.01f);

}

#endif // BUILD_MODE