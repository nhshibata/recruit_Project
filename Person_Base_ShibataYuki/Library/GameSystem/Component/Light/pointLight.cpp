//=========================================================
// [light.cpp]
// 作成:2022/06/27
// 
//=========================================================

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Light/spotLight.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Manager/sceneManager.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CPointLight::CPointLight()
{
}

//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CPointLight::CPointLight(std::shared_ptr<CGameObject> owner)
	:CLight(owner), m_fRange(1)
{
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");

	m_vColor = Color(rand() % 10 * 0.1f, rand() % 10 * 0.1f, rand() % 10 * 0.1f, 1.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CPointLight::~CPointLight()
{
	
}

//==========================================================
// 読み込み呼び出し
//==========================================================
void CPointLight::OnLoad()
{
	
}

//==========================================================
// 生成時呼び出し
//==========================================================
void CPointLight::Awake()
{
	CLight::Awake();
	GetOwner()->GetTagPtr()->CreateTag("light");
	GetOwner()->SetObjTag("light");
	
}

//========================================================
// 光の影響を受けるか
//========================================================
bool CPointLight::IsAffected(const Vector3 Apos, const float aRadius)const
{
	auto Bpos = Transform()->GetPos();
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = aRadius + GetRange();

	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

#ifdef BUILD_MODE

void CPointLight::ImGuiDebug()
{
	CLight::ImGuiDebug();

	ImGui::Separator();

	Debug::SetTextAndAligned("Light Range");
	ImGui::DragFloat3("##Range", (float*)&m_fRange);
	
	Debug::SetTextAndAligned("Light Color");
	ImGui::DragFloat4("##Color", (float*)&m_vColor, 0.1f);
	
}

#endif // BUILD_MODE